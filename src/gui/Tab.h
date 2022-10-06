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

#ifndef TAB_H_
#define TAB_H_

#include <QObject>
#include <QTabWidget>
#include <QUrl>
#include <QMenu>
#include "FormTable.h"

class Form;
class MainWindow;

class Tab : public QTabWidget {
  Q_OBJECT

public:
	Tab(QWidget *p=0);
	virtual ~Tab();
	template <typename T = FormTable>
	void createForm(){
		auto f = new T(this);
		QObject::connect(f,SIGNAL(titleChanged(const QString&)), SLOT(setToolTip(const QString&)));
		addTab(f, f->getTitle());
		f->init();
		return;
	};

public slots:
	int createBrowser(const QUrl&);
	void createViewer(const QString& i);
	void createSpeedReader(const QString& i);
	void closeTab(int);
	void currentChanged (int index);
	void setToolTip(const QString&);
	void indexChange();

private slots:
    void contextMenuRequested(const QPoint &position);

private:


};

#endif /* TAB_H_ */
