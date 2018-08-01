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

#include "Index.h"
#include "Doc.h"
#include "global.h"

Index::Index(QObject *p) : QObject(p){


}

Index::~Index() {
	try {
		if(db) db->close();
	} catch (const Xapian::Error& err) {

	}
}

void Index::init(){
	try {
		db.reset(new Xapian::WritableDatabase(pmcApp->pathIdx().toStdString(),
					Xapian::DB_CREATE_OR_OPEN));
	} catch (const Xapian::Error& e) {
		//qDebug() << e.get_description().c_str();
	}

}

void Index::save(Doc& d){
	Xapian::TermGenerator indexer;
    Xapian::Stem stemmer("english");
    indexer.set_stemmer(stemmer);
	Xapian::Document doc;
	indexer.set_document(doc);
	QString text;
	d.toText(text);
	doc.add_term("XID" + d.getPmcid().toLower().toStdString());
	doc.add_value(0,d.getPmcid().toStdString());
	doc.add_value(1,d.getName().toStdString());
	doc.add_value(2,std::to_string(d.getPageCount()));
	indexer.index_text(text.toStdString());
	db->replace_document("XID" + d.getPmcid().toLower().toStdString(), doc);
	db->commit();

}

void Index::deleteDoc (Doc& d) {
	db->delete_document("XID" + d.getPmcid().toLower().toStdString());
	db->commit();
}

void Index::deleteDoc(const QString& id){
	db->delete_document("XID" + id.toLower().toStdString());
	db->commit();
}

