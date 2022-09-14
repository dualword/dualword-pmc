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

#include "app/IndexModel.h"
#include "TableView.h"
#include "app/global.h"
#include <QHeaderView>
#include <QDebug>

TableView::TableView (QWidget *p) : QTableView(p), idxmodel(new IndexModel(this)), row(0) {
	  resizeColumnsToContents();
	  verticalHeader()->hide();
	  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	  setContextMenuPolicy(Qt::CustomContextMenu);
	  setSortingEnabled(true);
	  setSelectionBehavior(QAbstractItemView::SelectRows);
	  setEditTriggers(QAbstractItemView::NoEditTriggers);
	  setSelectionMode(QAbstractItemView::SingleSelection);
	  setModel(idxmodel);
	  connect(this,SIGNAL(doubleClicked(QModelIndex)), SLOT(doubleClicked(QModelIndex)));
	  connect(this->horizontalHeader(),QOverload<int, Qt::SortOrder>::of(&QHeaderView::sortIndicatorChanged),[=]() {
		  selectRow(std::min(row, idxmodel->count()-1));
		});
	  selectRow(0);
}

TableView::~TableView() {

}

void TableView::keyPressEvent(QKeyEvent *event) {
	if(event->key() == Qt::Key_Return){
		if(currentIndex().row() != -1) doubleClicked(currentIndex());
		return;
	}
	QTableView::keyPressEvent(event);
}

void TableView::doubleClicked(QModelIndex index) {
	row = index.row();
	mainWin->getTab()->createViewer((model()->sibling(index.row(),0,index)).data().toString());
}

void TableView::currentChanged ( const QModelIndex & current, const QModelIndex & previous ) {
	row = current.row();
	emit clicked((model()->sibling(current.row(),0,current)).data().toString());
	QAbstractItemView::currentChanged(current, previous);
}

void TableView::refresh(){
	idxmodel->refresh();
	selectRow(std::min(row, idxmodel->count()-1));
}

int TableView::count() {
	return idxmodel->count();
}

void TableView::setQuery(const QString& query) {
	idxmodel->setQuery(query);
	selectRow(0);
}

void TableView::setSort(bool b){
	setSortingEnabled(!b);
	idxmodel->setSort(b);
	selectRow(0);
	if(!b){
		connect(this->horizontalHeader(),QOverload<int, Qt::SortOrder>::of(&QHeaderView::sortIndicatorChanged),
				[=](){selectRow(std::min(row, idxmodel->count()-1));
		});
	}
}
