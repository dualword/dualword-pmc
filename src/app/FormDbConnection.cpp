/*
 *	Dualword-pmc is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Dualword-pmc is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Dualword-pmc. If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include "FormDbConnection.h"
#include "app/Doc.h"

#include "global.h"

FormDbConnection::FormDbConnection(QObject *p): QObject(p) {

}

FormDbConnection::~FormDbConnection() {
	db.close();
	db = QSqlDatabase();
	QSqlDatabase::removeDatabase(name);
}

void FormDbConnection::open(){
	name = QString ("dbc-");
	name.append( QString::number( reinterpret_cast<int>(this) ) );

	db = QSqlDatabase::addDatabase("QSQLITE", name);
	db.setDatabaseName(pmcApp->pathDb());

	if (!db.open()) {
		throw dualword_exception(db.lastError().text().toStdString());
	}

	QSqlQuery q(db);
	if(!q.exec("CREATE TABLE IF NOT EXISTS doc (" \
			"pmcid TEXT NOT NULL UNIQUE CHECK(length(pmcid) > 0)," \
			"name TEXT NOT NULL UNIQUE CHECK(length(name) > 0)," \
			"size INTEGER NOT NULL, pages INTEGER, data BLOB NOT NULL," \
			"created TEXT default CURRENT_TIMESTAMP, updated TEXT);")){
		throw dualword_exception("Error creating table");
	}
	q.finish();
}

void FormDbConnection::saveDoc(const Doc &doc){
	try {
		db.transaction();
		QSqlQuery q(db);

		q.prepare("INSERT INTO doc (pmcid, name,size,pages,data) "
					   "VALUES (:pmcid, :name, :size, :pages, :data)");

		q.bindValue(":pmcid", doc.getPmcid());
		q.bindValue(":name", doc.getName());
		q.bindValue(":size", doc.getSize());
		q.bindValue(":pages", doc.getPageCount());
		q.bindValue(":data", doc.getData());

		if(!q.exec()){
		  throw dualword_exception(q.lastError().text().toStdString());
		}
		q.finish();
		db.commit();
	} catch (const dualword_exception& e) {
		db.rollback();
		throw dualword_exception(e.what());
	} catch (const std::exception& e) {
		db.rollback();
		throw dualword_exception(e.what());
	}
}

void FormDbConnection::getDoc(Doc &d, const QString& id){
	QSqlQuery query(db);
	query.prepare("SELECT pmcid, name,size,data FROM doc where pmcid = :id" );
	query.bindValue(":id", id);

	if(!query.exec()){
	  throw dualword_exception(query.lastError().text().toStdString());
	}

	if(query.next()){
		d.setPmcid(query.value(0).toString());
		d.setName(query.value(1).toString());
		d.setSize(query.value(2).toInt());
		d.setData(query.value(3).toByteArray());
		d.open();
	}
	query.finish();

}

bool FormDbConnection::getNextDoc(Doc &d){
	QSqlQuery query("SELECT pmcid,name,size,data FROM doc WHERE updated is NULL limit 1",db);

	if(!query.exec()){
	  throw dualword_exception(query.lastError().text().toStdString());
	}

	if(query.next()){
		try {
			d.setPmcid(query.value(0).toString());
			d.setName(query.value(1).toString());
			d.setSize(query.value(2).toInt());
			d.setData(query.value(3).toByteArray());
			d.open();
		} catch(const dualword_exception& e) {
			query.finish();
			return false;
		}
		query.finish();
		return true;
	}
	query.finish();
	return false;
}

void FormDbConnection::deleteDoc(const QString& id){

	try {
		db.transaction();
		QSqlQuery q(db);
		q.prepare("DELETE FROM doc where pmcid = :id");
		q.bindValue(":id", id);

		if(!q.exec()){
		  throw dualword_exception(q.lastError().text().toStdString());
		}
		q.finish();
		db.commit();
	} catch (const dualword_exception& e) {
		db.rollback();
		throw dualword_exception(e.what());
	}
}

void FormDbConnection::updateDoc(const QString& id){
	try {
		db.transaction();
		QSqlQuery q(db);
		q.prepare("UPDATE doc set updated=CURRENT_TIMESTAMP where pmcid = :id");
		q.bindValue(":id", id);

		if(!q.exec()){
		  throw dualword_exception(q.lastError().text().toStdString());
		}
		q.finish();
		db.commit();
	} catch (const dualword_exception& e) {
		db.rollback();
		throw dualword_exception(e.what());
	}
}

bool FormDbConnection::exists(const QString& name){
	QSqlQuery q(db);
	q.prepare("SELECT * FROM doc where name = :name");
	q.bindValue(":name", name);

	q.exec();
	int i = q.size();
	q.finish();
	if(i <= 0) return false;
	return true;
}
