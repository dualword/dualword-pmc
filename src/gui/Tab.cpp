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

#include "app/global.h"
#include "Tab.h"
#include "Form.h"
#include "FormBrowser.h"
#include "FormTable.h"
#include "FormViewer.h"
#include "Browser.h"

Tab::Tab(QWidget *p) : QTabWidget(p) {
	setTabsClosable(true);
	setElideMode(Qt::ElideRight);
	setContextMenuPolicy(Qt::CustomContextMenu);
    setTabsClosable(true);
    setMovable(true);
    connect(this, SIGNAL(currentChanged (int)), SLOT(currentChanged(int)));
    connect(this, &QTabWidget::tabCloseRequested, this, &Tab::closeTab);
    connect(this, &QTabWidget::customContextMenuRequested, this, &Tab::contextMenuRequested);
}

Tab::~Tab() {

}

void Tab::contextMenuRequested(const QPoint &position) {
	QMenu menu;
    menu.addAction(tr("Browser Tab"), [this]{createForm<FormBrowser>();});
    menu.addAction(tr("Table"), [this]{createForm();});
    menu.exec(QCursor::pos());
}

int Tab::createBrowser(const QUrl& url){
	auto f = new FormBrowser(this);
    QObject::connect(f,SIGNAL(titleChanged(const QString&)), SLOT(setToolTip(const QString&)));
    f->getBrowser()->load(url);
	return addTab(f,"Browser");
}

void Tab::createViewer(const QString& i){
	auto f = new FormViewer(this);
	f->init();
	f->loadDoc(i);
	addTab(f,f->getName());
}

void Tab::closeTab(int i){
	if(this->count() == 1) createForm();
	widget(i)->deleteLater();
	removeTab(i);
}

void Tab::currentChanged (int index){
	auto f = qobject_cast<Form*>(currentWidget());
    QObject::connect(f,SIGNAL(statusBarMessage(const QString&)),
    		mainWin->statusBar(), SLOT(showMessage(const QString&)), Qt::UniqueConnection);
}

void Tab::setToolTip(const QString& s){
	auto f = qobject_cast<Form*>(sender());
	int i = indexOf(f);
	if (i != -1) {
		setTabText(i, s.mid(0,20));
		setTabToolTip(i, s);
	}
}

void Tab::indexChange(){
	auto f = qobject_cast<Form*>(currentWidget());
	if(qobject_cast<FormTable*>(f)) (qobject_cast<FormTable*>(f))->refresh();
}

