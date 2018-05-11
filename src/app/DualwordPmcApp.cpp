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

#include <QFile>
#include "DualwordPmcApp.h"
#include "gui/MainWindow.h"

DualwordPmcApp::DualwordPmcApp(int &argc, char **argv) : QApplication(argc, argv) {
	setApplicationName("Dualword-pmc");
	#ifdef _VER
		QApplication::setApplicationVersion(_VER);
	#endif
	QApplication::setQuitOnLastWindowClosed(true);

}

DualwordPmcApp::~DualwordPmcApp() {

}

DualwordPmcApp *DualwordPmcApp::instance() {
    return (static_cast<DualwordPmcApp *>(QCoreApplication::instance()));
}

QString DualwordPmcApp::getHtml(const QString& f){
	QFile file;
	file.setFileName(f);
	bool ok = file.open(QIODevice::ReadOnly);
	QString html = QString(QLatin1String(file.readAll()));
	return html;
}

void DualwordPmcApp::start() {
	win = new MainWindow();
	win->init();
	win->show();
}

