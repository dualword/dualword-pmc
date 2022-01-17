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
#include <QMutex>
#include <QThreadPool>

class MainWindow;
class Indexer;
class Index;

class DualwordPmcApp : public QApplication {
    Q_OBJECT

public:
	DualwordPmcApp() = delete;
	DualwordPmcApp(const DualwordPmcApp&) = delete;
	DualwordPmcApp& operator=(const DualwordPmcApp&) = delete;
	DualwordPmcApp(int &argc, char **argv);
	virtual ~DualwordPmcApp();
	MainWindow* window() {return w;};
	Index* index() {return idx.get();};
    static DualwordPmcApp *instance();
    static QString getHtml(const QString&);

    template <typename T, typename P>
    void startTask(P p) {
    	QThreadPool::globalInstance()->start(new T(p));
    };

	const QString& pathApp() const {
		return appPath;
	}

	const QString& pathIdx() const {
		return idxPath;
	}

	const QString& pathDb() const {
		return dbPath;
	}

public slots:
	void start();
	void startIndexer();
	void stopIndexer();

private:
	void createDb();
	void createIndex();

private:
	static const QString appPath, dbPath, idxPath;

	MainWindow* w;
	QScopedPointer<Indexer> indexer;
	QScopedPointer<Index> idx;
    QMutex lock;
};

#endif /* APP_DUALWORDPMCAPP_H_ */
