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

#include "app/DualwordPmcApp.h"
#include "gui/MainWindow.h"
#include "Tab.h"
#include "Form.h"
#include "FormBrowser.h"
#include "Browser.h"

Tab::Tab(QWidget *p) : QTabWidget(p) {
	setTabsClosable(true);
	setElideMode(Qt::ElideRight);
	setContextMenuPolicy(Qt::CustomContextMenu);
    setTabsClosable(true);
    setMovable(true);
    connect(this, SIGNAL(currentChanged (int)), SLOT(currentChanged(int)));
    connect(this, SIGNAL(tabCloseRequested (int)), SLOT(closeTab(int)));
    connect(this, SIGNAL(NewBrowser()), SLOT(createBrowser()));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
    		SLOT(contextMenuRequested(QPoint)));
}

Tab::~Tab() {

}

void Tab::contextMenuRequested(const QPoint &position) {
	QMenu menu;
    menu.addAction(tr("Browser Tab"), this, SIGNAL(NewBrowser()));
    menu.exec(QCursor::pos());
}

int Tab::createBrowser(){
	auto f = new FormBrowser(this);
    QObject::connect(f,SIGNAL(titleChanged(const QString&)), SLOT(setToolTip(const QString&)));
	return addTab(f,"Browser");
}

int Tab::createBrowser(const QUrl& url){
	auto f = new FormBrowser(this);
    QObject::connect(f,SIGNAL(titleChanged(const QString&)), SLOT(setToolTip(const QString&)));
    f->getBrowser()->load(url);
	return addTab(f,"Browser");
}

void Tab::closeTab(int i){
	if(this->count() == 1) createBrowser();
	removeTab(i);
}

void Tab::currentChanged (int index){
	auto f = qobject_cast<Form*>(currentWidget());
    QObject::connect(f,SIGNAL(statusBarMessage(const QString&)),
    		DualwordPmcApp::instance()->window()->statusBar(),
			SLOT(showMessage(const QString&)), Qt::UniqueConnection);
}

void Tab::setToolTip(const QString& s){
	auto f = qobject_cast<Form*>(sender());
	int i = indexOf(f);
	if (i != -1) {
		setTabText(i, f->getTitle());
		setTabToolTip(i, f->getTitle());
	}
}
