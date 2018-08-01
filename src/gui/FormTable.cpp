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

#include "app/Index.h"
#include "gui/TableView.h"
#include "app/global.h"
#include "FormTable.h"
#include "app/FormDbConnection.h"
#include "gui/FormViewer.h"

FormTable::FormTable(QWidget *p) : Form(p) {
	createUi();
	connectSlots();
}

FormTable::~FormTable() {

}

void FormTable::init(){
	  db = new FormDbConnection(this);
	  db->open();
}

void FormTable::createUi(){
	view = new FormViewer(this);
	view->init();

	table = new TableView(this);
	connect(table,SIGNAL(customContextMenuRequested(const QPoint)),
			this,SLOT(contextMenuRequested(QPoint)));

	menu = new QMenu(table);
	m_del = menu->addAction("Delete");
	connect(m_del,SIGNAL(triggered()), SLOT(deleteDoc()));
	QAction *a = menu->addAction(tr("Open in browser"), this, SLOT(openDoc()));

	QHBoxLayout *bl = new QHBoxLayout;
	search = new QLineEdit(this);
	QObject::connect(search, SIGNAL(returnPressed()), SLOT(doSearch()));
	bl->addWidget(search);
	count = new QLineEdit(this);
	count->setAlignment(Qt::AlignHCenter);
	count->setMaximumWidth(100);
	bl->addWidget(count);

	QSizePolicy policy = view->sizePolicy();
	policy.setHorizontalStretch(1);
	view->setSizePolicy(policy);

	QSplitter* sp = new QSplitter(this);
	sp->addWidget(table);
	sp->addWidget(view);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addLayout(bl);
	layout->addWidget(sp);
	setLayout(layout);
}

void FormTable::connectSlots(){
	connect(table,SIGNAL(clicked(const QString&)), view, SLOT(loadDoc(const QString&)));
}

void FormTable::contextMenuRequested(QPoint p){
	menu->popup(mapToGlobal(p));
}

void FormTable::deleteDoc(){
	QModelIndex i;
	QString id = (table->model()->sibling(table->selectionModel()->currentIndex().row(),0,i)).data().toString();
	db->deleteDoc(id);
	idx->deleteDoc(id);
	table->refresh();
	count->setText(QString::number(table->count()));

}

void FormTable::openDoc(){
	QModelIndex i;
	mainWin->getTab()->createBrowser(QUrl("http://www.ncbi.nlm.nih.gov/pmc/" +
			(table->model()->sibling(table->selectionModel()->currentIndex().row(),0,i)).data().toString()));
}

QString FormTable::getTitle() const{
	return "";
}

void FormTable::showEvent(QShowEvent *e){
	table->refresh();
	count->setText(QString::number(table->count()));
	QWidget::showEvent(e);
}

void FormTable::hideEvent(QHideEvent *e){
	QWidget::hideEvent(e);
}

void FormTable::doSearch(){
	table->setQuery(search->text().trimmed());
	count->setText(QString::number(table->count()));
	emit titleChanged(search->text().length()>0 ? search->text() : "Table");
}
