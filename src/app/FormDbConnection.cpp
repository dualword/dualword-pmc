/*
 *	Dualword-PMC is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Dualword-PMC is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Dualword-PMC. If not, see <http://www.gnu.org/licenses/>.
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

FormDbConnection* FormDbConnection::open(){
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
	execSql("PRAGMA mmap_size = 10000000000");
	execSql("PRAGMA incremental_vacuum(10000000)");
	return this;
}

void FormDbConnection::create(){
	execSql("PRAGMA page_size = 32768");
	execSql("PRAGMA auto_vacuum = 2");

	execSql("CREATE TABLE IF NOT EXISTS doc (" \
			"id INTEGER PRIMARY KEY AUTOINCREMENT," \
			"pmcid TEXT NOT NULL UNIQUE CHECK(length(pmcid) > 0)," \
			"name TEXT NOT NULL CHECK(length(name) > 0)," \
			"size INTEGER NOT NULL, pages INTEGER, images INTEGER," \
			"created TEXT default CURRENT_TIMESTAMP, updated TEXT);");

	execSql("CREATE TABLE IF NOT EXISTS pdf (" \
			"id INTEGER PRIMARY KEY REFERENCES doc(id) ON DELETE CASCADE," \
			"data BLOB NOT NULL);");

	execSql("CREATE TABLE IF NOT EXISTS history (" \
			"text TEXT NOT NULL UNIQUE CHECK(length(text) > 0)," \
			"updated TEXT default CURRENT_TIMESTAMP);");

	execSql("CREATE VIEW IF NOT EXISTS v_pdf AS " \
			"select pmcid,name,size,created,updated,data " \
			"FROM doc d INNER JOIN pdf p ON d.id=p.id;");
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
		q.prepare("INSERT INTO pdf (id, data) VALUES (:id, :data)");
		q.bindValue(":id", id);
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
	query.prepare("SELECT pmcid,name,size,data,created FROM v_pdf where pmcid = :id" );
	query.bindValue(":id", id);

	if(!query.exec()){
	  throw dualword_exception(query.lastError().text().toStdString());
	}

	if(query.next()){
		d.setPmcid(query.value(0).toString());
		d.setName(query.value(1).toString());
		d.setSize(query.value(2).toInt());
		d.setData(query.value(3).toByteArray());
		d.setCreated(query.value(4).toDateTime().toMSecsSinceEpoch());
	}
	query.finish();

}

bool FormDbConnection::getNextDoc(Doc &d){
	QSqlQuery query("SELECT pmcid,name,size,data,created FROM v_pdf WHERE updated is NULL limit 1",db);

	if(!query.exec()){
	  throw dualword_exception(query.lastError().text().toStdString());
	}

	if(query.next()){
		try {
			d.setPmcid(query.value(0).toString());
			d.setName(query.value(1).toString());
			d.setSize(query.value(2).toInt());
			d.setData(query.value(3).toByteArray());
			d.setCreated(query.value(4).toDateTime().toMSecsSinceEpoch());
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

bool FormDbConnection::getNextDelete(Doc &d){
	QSqlQuery query("SELECT pmcid FROM v_pdf WHERE created is NULL limit 1",db);

	if(!query.exec()){
	  throw dualword_exception(query.lastError().text().toStdString());
	}

	if(query.next()){
		try {
			d.setPmcid(query.value(0).toString());
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

bool FormDbConnection::exists(const QString& id){
	bool b = false;
	QSqlQuery q(db);
	q.setForwardOnly(true);
	q.prepare("SELECT * FROM doc where pmcid = :id");
	q.bindValue(":id", id);
	q.exec();
	b = q.next();
	q.finish();
	return b;
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

void FormDbConnection::reindex(const QString& id){
	try {
		db.transaction();
		QSqlQuery q(db);
		q.prepare("UPDATE doc SET updated = NULL WHERE pmcid = :id");
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

void FormDbConnection::remove(const QString& id){
	try {
		db.transaction();
		QSqlQuery q(db);
		q.prepare("UPDATE doc SET created = NULL WHERE pmcid = :id");
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

QStringList FormDbConnection::searchHistory(){
	QStringList list;
	QSqlQuery q("SELECT text FROM history", db);
	q.setForwardOnly(true);
	q.exec();
	while (q.next())
		list << q.value(0).toString();
	q.finish();
	return list;
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

int FormDbConnection::getId(const QString& id){
	QSqlQuery q("select id from doc where pmcid='"+id+"'", db);
	q.setForwardOnly(true);
	q.exec();
	q.first();
	return q.value(0).toInt();
}

void FormDbConnection::clearHistory(){
	db.transaction();
	QSqlQuery q(db);
	q.exec("DELETE from history");
	q.finish();
	db.commit();
}

