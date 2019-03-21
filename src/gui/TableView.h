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

#ifndef GUI_TABLEVIEW_H_
#define GUI_TABLEVIEW_H_

#include <QTableView>

class IndexModel;

class TableView : public QTableView {
	Q_OBJECT

public:
	TableView(QWidget * p = 0);
	virtual ~TableView();
	int count();
	void refresh();

public slots:
	void doubleClicked(QModelIndex index);
	void setQuery(const QString&);
	void setSort(bool);

signals:
	void doubleClicked(const QString&);
	void clicked(const QString&);

protected:
	void currentChanged (const QModelIndex&, const QModelIndex& );
    void keyPressEvent(QKeyEvent *event);

private:
	IndexModel *idxmodel;
	int row;

};

#endif /* GUI_TABLEVIEW_H_ */
