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
	searchModel->setStringList(db->searchHistory());
	table->setFocus();
}

void FormTable::createUi(){
	view = new FormViewer(this);
	view->setLoadImages(false);
	view->init();

	table = new TableView(this);
	connect(table,SIGNAL(customContextMenuRequested(const QPoint&)),
			SLOT(contextMenuRequested(const QPoint&)));

	QHBoxLayout *bl = new QHBoxLayout();
	search = new QLineEdit(this);
	QObject::connect(search, SIGNAL(returnPressed()), SLOT(doSearch()));
	completer = new QCompleter(this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	searchModel = new QStringListModel(search);
	completer->setModel(searchModel);
	search->setCompleter(completer);

	sort = new QCheckBox(this);
	sort->setToolTip("Sort by relevance");
	QObject::connect(sort, SIGNAL(stateChanged(int)), SLOT(setSort()));
	bl->addWidget(search);
	count = new QLineEdit(this);
	count->setAlignment(Qt::AlignHCenter);
	count->setMaximumWidth(100);
	bl->addWidget(count);
	bl->addWidget(sort);

	QSizePolicy policy = view->sizePolicy();
	policy.setHorizontalStretch(1);
	view->setSizePolicy(policy);

	QSplitter* sp = new QSplitter(this);
	sp->addWidget(table);
	sp->addWidget(view);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	layout->addLayout(bl);
	layout->addWidget(sp);
	setLayout(layout);
}

void FormTable::connectSlots(){
	connect(table,SIGNAL(clicked(const QString&)), view, SLOT(loadDoc(const QString&)));
}

void FormTable::contextMenuRequested(const QPoint& p){
	QMenu menu(this);
	menu.addAction(tr("Open"), this, SLOT(openDoc()));
	menu.addAction(tr("Open in browser"), this, SLOT(openUrl()));
	menu.addAction(tr("Save PDF"), this, SLOT(saveDoc()));
	menu.addAction(tr("Delete"), this, SLOT(deleteDoc()));
	menu.exec(QCursor::pos());
}

void FormTable::deleteDoc(){
	if(table->count() <= 0) return;
	if (!mainWin->askYesNo(this, "Delete PDF?")) return;
	QModelIndex i;
	QString id = (table->model()->sibling(table->selectionModel()->currentIndex().row(),0,i)).data().toString();
	db->deleteDoc(id);
	idx->deleteDoc(id);
	table->refresh();
	count->setText(QString::number(table->count()));

}

void FormTable::openUrl(){
	if(table->count() <= 0) return;
	QModelIndex i;
	mainWin->getTab()->createBrowser(QUrl("http://www.ncbi.nlm.nih.gov/pmc/" +
			(table->model()->sibling(table->selectionModel()->currentIndex().row(),0,i)).data().toString()));
}

QString FormTable::getTitle() const{
	return "";
}

void FormTable::showEvent(QShowEvent *e){
	refresh();
	QWidget::showEvent(e);
}

void FormTable::hideEvent(QHideEvent *e){
	QWidget::hideEvent(e);
}

void FormTable::doSearch(){
	view->loadDoc("-1");
	table->setQuery(search->text().trimmed());
	count->setText(QString::number(table->count()));
	emit titleChanged(search->text().trimmed().length()>0 ? search->text() : "Table");
	if(search->text().trimmed().length() > 0) db->saveSearch(search->text().trimmed());
	searchModel->setStringList(db->searchHistory());
	table->setFocus();
}

void FormTable::refresh(){
	table->refresh();
	count->setText(QString::number(table->count()));
	searchModel->setStringList(db->searchHistory());
}

void FormTable::saveDoc(){
	if(table->count() <= 0) return;
	QModelIndex i;
	QString id = (table->model()->sibling(table->selectionModel()->currentIndex().row(),0,i)).data().toString();
	Doc doc;
	db->getDoc(doc, id);
	if(doc.getPmcid().length() <= 0) return;
	QFileDialog d(this,tr("Choose directory"), QDir::homePath());
	d.setFilter(QDir::Dirs | QDir::Hidden);
	d.setFileMode(QFileDialog::DirectoryOnly);
	d.setViewMode(QFileDialog::List);
	d.setOption(QFileDialog::ShowDirsOnly,true);
	d.setOption(QFileDialog::ReadOnly,true);
    if (d.exec() == QDialog::Accepted) {
        QString dir = QString(d.selectedFiles().value(0));
        QString fname = dir.append(QDir::separator()).append(doc.getName());
        QFile f(fname);
        f.open(QIODevice::WriteOnly);
      	f.write(doc.getData());
      	f.flush();
      	f.close();
        mainWin->statusBar()->showMessage("PDF saved in " + fname, 5000);
    }
}

void FormTable::openDoc(){
	if(table->count() <= 0) return;
	QModelIndex i;
	mainWin->getTab()->createViewer((table->model()->sibling(table->selectionModel()->currentIndex().row(),0,i)).data().toString());
}

void FormTable::setSort(){
	table->setSort(sort->isChecked());
	table->setFocus();
}
