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

#ifndef APP_INDEXMODEL_H_
#define APP_INDEXMODEL_H_

#include <xapian.h>
#include <QAbstractTableModel>
#include <QStringList>

class IndexModel : public QAbstractTableModel {
	Q_OBJECT

public:
	IndexModel(QObject * p = 0);
	virtual ~IndexModel();
	void init();
	void refresh();
	int count();

	QVariant headerData ( int , Qt::Orientation, int role = Qt::DisplayRole ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	int	rowCount ( const QModelIndex & p = QModelIndex() ) const;
	int	columnCount ( const QModelIndex & p = QModelIndex() ) const;
	void setQuery(const QString& query);

private:
	Xapian::Database db;
	Xapian::MSet set;
	QString query;
	QStringList list;

};

#endif /* APP_INDEXMODEL_H_ */
