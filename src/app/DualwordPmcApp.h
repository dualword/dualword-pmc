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

#ifndef APP_DUALWORDPMCAPP_H_
#define APP_DUALWORDPMCAPP_H_

#include <QApplication>

class MainWindow;

class DualwordPmcApp : public QApplication {
    Q_OBJECT

public:
	DualwordPmcApp(int &argc, char **argv);
	virtual ~DualwordPmcApp();
	MainWindow* window() {return win;};
    static DualwordPmcApp *instance();
    static QString getHtml(const QString&);

public slots:
	void start();

private:
	MainWindow* win;
};

#endif /* APP_DUALWORDPMCAPP_H_ */
