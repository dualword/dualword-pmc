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
		//throw dualword_exception(db.lastError().text().toStdString());
	}

	execSql("PRAGMA synchronous = OFF");
	execSql("PRAGMA journal_mode = MEMORY");
	execSql("PRAGMA temp_store = MEMORY");
	execSql("PRAGMA foreign_keys = ON");
	execSql("PRAGMA auto_vacuum = 2");

}

void FormDbConnection::create(){

	execSql("CREATE TABLE IF NOT EXISTS doc (" \
			"id INTEGER PRIMARY KEY AUTOINCREMENT," \
			"pmcid TEXT NOT NULL UNIQUE CHECK(length(pmcid) > 0)," \
			"name TEXT NOT NULL UNIQUE CHECK(length(name) > 0)," \
			"size INTEGER NOT NULL, pages INTEGER, images INTEGER," \
			"created TEXT default CURRENT_TIMESTAMP, updated TEXT);");

	execSql("CREATE TABLE IF NOT EXISTS pdf (" \
			"id INTEGER PRIMARY KEY AUTOINCREMENT," \
			"docid INTEGER NOT NULL," \
			"data BLOB NOT NULL," \
			"FOREIGN KEY(docid) REFERENCES doc(id) ON DELETE CASCADE );");

	execSql("CREATE TABLE IF NOT EXISTS history (" \
			"text TEXT NOT NULL UNIQUE CHECK(length(text) > 0)," \
			"updated TEXT default CURRENT_TIMESTAMP);");

	execSql("CREATE VIEW IF NOT EXISTS v_pdf AS " \
			"select pmcid,name,size,updated,data " \
			"FROM doc d INNER JOIN pdf p ON d.id=p.docid;");

	execSql("PRAGMA incremental_vacuum(1000000)");
}

void FormDbConnection::execSql(const QString& sql){
	QSqlQuery q(db);
	if(!q.exec(sql)){
		//throw dualword_exception(q.lastError().text().toStdString());
	}
	q.finish();
}

void FormDbConnection::saveDoc(const Doc &doc){
	try {
		db.transaction();
		QSqlQuery q(db);

		q.prepare("INSERT INTO doc (pmcid, name,size,pages) "
					   "VALUES (:pmcid, :name, :size, :pages)");

		q.bindValue(":pmcid", doc.getPmcid());
		q.bindValue(":name", doc.getName());
		q.bindValue(":size", doc.getSize());
		q.bindValue(":pages", doc.getPageCount());

		if(!q.exec()){
		  throw dualword_exception(q.lastError().text().toStdString());
		}
		int id = q.lastInsertId().toInt();
		q.prepare("INSERT INTO pdf (docid, data) VALUES (:docid, :data)");
		q.bindValue(":docid", id);
		q.bindValue(":data", doc.getData());
		if(!q.exec()){
		  throw dualword_exception(q.lastError().text().toStdString());
		}
		q.finish();
		db.commit();
	} catch (const dualword_exception& e) {
		db.rollback();
		//throw dualword_exception(e.what());
	}
}

void FormDbConnection::getDoc(Doc &d, const QString& id){
	QSqlQuery query(db);
	query.prepare("SELECT pmcid,name,size,data FROM v_pdf where pmcid = :id" );
	query.bindValue(":id", id);

	if(!query.exec()){
	  throw dualword_exception(query.lastError().text().toStdString());
	}

	if(query.next()){
		d.setPmcid(query.value(0).toString());
		d.setName(query.value(1).toString());
		d.setSize(query.value(2).toInt());
		d.setData(query.value(3).toByteArray());
	}
	query.finish();

}

bool FormDbConnection::getNextDoc(Doc &d){
	QSqlQuery query("SELECT pmcid,name,size,data FROM v_pdf WHERE updated is NULL limit 1",db);

	if(!query.exec()){
	  throw dualword_exception(query.lastError().text().toStdString());
	}

	if(query.next()){
		try {
			d.setPmcid(query.value(0).toString());
			d.setName(query.value(1).toString());
			d.setSize(query.value(2).toInt());
			d.setData(query.value(3).toByteArray());
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
		//throw dualword_exception(e.what());
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
		//throw dualword_exception(e.what());
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

void FormDbConnection::reindex(){
	try {
		db.transaction();
		QSqlQuery q(db);
		q.prepare("UPDATE doc SET updated = NULL");
		if(!q.exec()){
		  throw dualword_exception(q.lastError().text().toStdString());
		}
		q.finish();
		db.commit();
	} catch (const dualword_exception& e) {
		db.rollback();
		//throw dualword_exception(e.what());
	}
}

void FormDbConnection::saveSearch(const QString& str){
	db.transaction();
	QSqlQuery q(db);
	q.prepare("INSERT INTO history (text) VALUES (:text)");
	q.bindValue(":text", str);
	q.exec();
	q.prepare("UPDATE history SET updated=CURRENT_TIMESTAMP where text = :text");
	q.bindValue(":text", str);
	q.exec();
	if (getCount("history") > 1000){
		q.exec("DELETE from history where rowid = "
				"(select rowid from history order by updated limit 1)");
	}
	q.finish();
	db.commit();
}

void FormDbConnection::searchHistory(QStringList& list){
	QSqlQuery q("SELECT text FROM history", db);
	q.setForwardOnly(true);
	q.exec();
	while (q.next())
		list << q.value(0).toString();
	q.finish();
}

int FormDbConnection::getCount(const QString& t){
	QSqlQuery q("SELECT count(*) FROM " + t, db);
	q.setForwardOnly(true);
	q.exec();
	int i = 0;
	if (q.first())
		i = q.value(0).toInt();
	q.finish();
	return i;
}

bool FormDbConnection::getValue(const QString& sql, QString& str){
	bool b = false;
	QSqlQuery q(sql, db);
	q.setForwardOnly(true);
	q.exec();
	if (q.first()){
		str = q.value(0).toString();
		b = true;
	}
	q.finish();
	return b;
}

void FormDbConnection::clearHistory(){
	db.transaction();
	QSqlQuery q(db);
	q.exec("DELETE from history");
	q.finish();
	db.commit();
}

