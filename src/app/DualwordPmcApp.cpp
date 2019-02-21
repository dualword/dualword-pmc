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
#include "app/Indexer.h"
#include <QFile>
#include "app/global.h"

const QString DualwordPmcApp::appPath = QDir::homePath().append(QDir::separator()).append(".dualword-pmc")
.append(QDir::separator());

const QString DualwordPmcApp::dbPath = appPath + "dualword-pmc.sqlite";

const QString DualwordPmcApp::idxPath = appPath + "index";

DualwordPmcApp::DualwordPmcApp(int &argc, char **argv) : QApplication(argc, argv), indexer(0), idx(0), lock() {
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
	createIndex();
	createDb();
	w = new MainWindow();
	w->init();
	w->show();
	startIndexer();
}

void DualwordPmcApp::startIndexer(){
	QMutexLocker locker(&lock);
	if(indexer && indexer->isRunning()) return;
	indexer.reset(new Indexer(this));
	QObject::connect(indexer.data(), SIGNAL(newMsg(const QString&)), mainWin->statusBar(), SLOT(showMessage(const QString&)));
	indexer->start(QThread::LowestPriority);

}

void DualwordPmcApp::createDb(){

}

void DualwordPmcApp::createIndex(){
	QDir().mkpath(idxPath);
	idx.reset(new Index(this));
	idx->init();
}

