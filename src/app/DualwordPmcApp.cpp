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

#include "public/fpdfview.h"
#include <QtWebEngineWidgets/QtWebEngineWidgets>

const QString DualwordPmcApp::appPath = QDir::homePath().append(QDir::separator()).append(".dualword-pmc")
.append(QDir::separator());

const QString DualwordPmcApp::dbPath = appPath + "dualword-pmc.sqlite";

const QString DualwordPmcApp::idxPath = appPath + "index";

DualwordPmcApp::DualwordPmcApp(int &argc, char **argv) : QApplication(argc, argv), indexer(0), idx(0), lock() {
	setApplicationName("Dualword-PMC");
	setOrganizationName("dualword");
	#ifdef _VER
		QApplication::setApplicationVersion(_VER);
	#endif
	QApplication::setQuitOnLastWindowClosed(true);
	clearWebHistory();
	FPDF_InitLibrary();
}

DualwordPmcApp::~DualwordPmcApp() {
	clearWebHistory();
	FPDF_DestroyLibrary();
	if(indexer->isRunning()){
		indexer->setStop(true);
		indexer->wait();
	}
}

DualwordPmcApp *DualwordPmcApp::instance() {
    return (static_cast<DualwordPmcApp *>(QCoreApplication::instance()));
}

QString DualwordPmcApp::getHtml(const QString& f){
	QFile file;
	file.setFileName(f);
	bool ok = file.open(QIODevice::ReadOnly);
	return QString(QLatin1String(file.readAll()));
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
	QObject::connect(indexer.get(), SIGNAL(newMsg(const QString&)), mainWin->statusBar(), SLOT(showMessage(const QString&)));
	indexer->start(QThread::LowestPriority);
}

void DualwordPmcApp::stopIndexer(){
	if(indexer->isRunning()){
		indexer->setStop(true);
	}
}

void DualwordPmcApp::createDb(){
	FormDbConnection db;
	db.open();
	db.create();
}

void DualwordPmcApp::createIndex(){
	QDir().mkpath(idxPath);
	idx.reset(new Index(this));
	idx->init();
}

void DualwordPmcApp::clearWebHistory(){
	QWebEngineProfile::defaultProfile()->cookieStore()->deleteAllCookies();
	QWebEngineProfile::defaultProfile()->clearHttpCache();
	QWebEngineProfile::defaultProfile()->clearAllVisitedLinks();
}

void DualwordPmcApp::setValue(const QString &key, const QVariant& val){
	QSettings s;
	s.setValue(key, val);
}

QVariant DualwordPmcApp::value(const QString &key, const QVariant& val){
	QSettings s;
	return s.value(key, val);
}
