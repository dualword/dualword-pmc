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

#include "MainWindow.h"
#include "Tab.h"
#include "app/global.h"

MainWindow::MainWindow(QWidget *p, Qt::WindowFlags f) : QMainWindow(p, f) {
	setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setSlots();
}

MainWindow::~MainWindow() {

}

void MainWindow::init(){
	tab->createTable();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}

void MainWindow::exit() {
	qApp->quit();
}

void MainWindow::showAbout() {
	QString str;
	str.append(qApp->applicationName());
	str.append(" ").append(qApp->applicationVersion()).append("<br>");
	str.append("License: GPL v3 <br/>");
	str.append("&copy;2018 Alexander Busorgin <br/>");
	QMessageBox::about(this, tr("About"), str );
}

void MainWindow::showInfo(){
	QString str;
	FormDbConnection db;
	db.open();
	str.append("<br/>PDF Documents: " + QString::number(db.getCount("doc")));
	str.append("<br/>Db: " + pmcApp->pathDb());
	str.append("<br/>Db size: " + QString::number(QFile(pmcApp->pathDb()).size()/1000/1000) + " MB.");
	QMessageBox::about(this, tr("Info"), str );
}

void MainWindow::reindex() {
	FormDbConnection db;
	db.open();
	db.reindex();
	pmcApp->startIndexer();
}

void MainWindow::clearHistory(){
	if (!mainWin->askYesNo(this, "Clear search history?")) return;
	FormDbConnection db;
	db.open();
	db.clearHistory();
	tab->indexChange();
}

bool MainWindow::askYesNo(QWidget* p, const QString& str){
	bool yes = false;
	QMessageBox::StandardButton r;
	r = QMessageBox::question(p, tr(""),str, QMessageBox::Yes | QMessageBox::No);
	if (r == QMessageBox::Yes) yes = true;
	return yes;
}
