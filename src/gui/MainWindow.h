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
#ifndef GUI_MAINWINDOW_H_
#define GUI_MAINWINDOW_H_

#include <QtWidgets>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindowForm {
  Q_OBJECT

public:
	MainWindow(QWidget *p=0, Qt::WindowFlags f=0);
	virtual ~MainWindow();
	void init();
	bool askYesNo(QWidget*, const QString&);
	Tab* getTab() { return tab; };

protected:
    void closeEvent(QCloseEvent *);

private slots:
	void setSlots();
	void showAbout();
	void showInfo();
	void exit();
	void reindex();
	void clearHistory();

};

#endif /* GUI_MAINWINDOW_H_ */
