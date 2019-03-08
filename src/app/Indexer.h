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

#ifndef APP_INDEXER_H_
#define APP_INDEXER_H_

#include <QThread>
#include <QDebug>

class Indexer : public QThread{
	Q_OBJECT

public:
	Indexer(QObject *p = 0);
	virtual ~Indexer();

	void setStop(bool b) {
		this->stop = b;
	}

signals:
	void newMsg(const QString&);

protected:
     void run();

private:
     bool stop;

};

#endif /* APP_INDEXER_H_ */
