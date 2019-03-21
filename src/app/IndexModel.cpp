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

#include "IndexModel.h"
#include "global.h"

IndexModel::IndexModel (QObject *p) : QAbstractTableModel(p),
	db(), set(), query(""), list(), sortColumn(0), sortAsc(0), sortType(0) {
	list << "PMCID" << "Name" << "Page" << "Image" << "Size" << "Date";
	init();
	setQuery("");
}

IndexModel::~IndexModel() {
	try {
		db.close();
	} catch (const Xapian::Error& err) {
		//
	}
}

void IndexModel::init(){
	try {
		db = Xapian::Database(QString(pmcApp->pathIdx()).toStdString());
	} catch (const Xapian::Error& e) {
		//qDebug() << e.get_msg().c_str();
	}
}

QVariant IndexModel::data (const QModelIndex& index, int role) const {
	if (!index.isValid()) return QVariant();
	if (role == Qt::DisplayRole) {
		try {
			Xapian::Document doc = set[index.row()].get_document();
			if(index.column()==2 || index.column()==3 || index.column()==4){
				return QString::number(Xapian::sortable_unserialise(doc.get_value(index.column())),'g',20);
			}else if(index.column()==5){
				return QDateTime::fromMSecsSinceEpoch((qint64)Xapian::sortable_unserialise(doc.get_value(index.column())));
			}else{
				return QString::fromStdString(doc.get_value(index.column()));
			}
		} catch (const Xapian::Error& e) {
			//
		}
	}
	return QVariant();
}

void IndexModel::sort (int column, Qt::SortOrder order){
	sortColumn = column;
	order == Qt::AscendingOrder ? sortAsc=true : sortAsc=false;
	refresh();
}

int	IndexModel::rowCount ( const QModelIndex& p ) const{
	return set.size();
}

int	IndexModel::columnCount ( const QModelIndex& p) const{
	return list.size();
}

QVariant IndexModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole) return QVariant();
	if(orientation == Qt::Horizontal){
		return list.at(section);
	}
	return QVariant();
}


void IndexModel::setQuery(const QString& query) {
	beginResetModel();
	Xapian::Query q(Xapian::Query::MatchAll);
	this->query = query;
	try{
		Xapian::Enquire enquire(db);
		if(sortType){
			enquire.set_sort_by_relevance();
		}else{
			enquire.set_sort_by_value(sortColumn, sortAsc);
		}
		Xapian::QueryParser qp;
	    Xapian::Stem stemmer("english");
	    qp.set_stemmer(stemmer);
	    qp.set_stemming_strategy(Xapian::QueryParser::STEM_SOME);
		qp.set_database(db);
		qp.add_prefix("id", "XID");
		qp.set_default_op(Xapian::Query::OP_AND);
		if(query.length() > 0) q = qp.parse_query(query.toStdString());
		enquire.set_query(q);
		set = enquire.get_mset(0, db.get_doccount());
		endResetModel();
	} catch (const Xapian::Error& e) {
		//
	}
}

void IndexModel::refresh(){
	db.reopen();
	setQuery(query);
}

int	IndexModel::count () {
	return set.size();
}

void IndexModel::setSort(bool b){
	sortType = b;
	refresh();
}
