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

#ifndef APP_INDEX_H_
#define APP_INDEX_H_

#include <xapian.h>

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QTimer>
#include <QSet>

class Doc;

class Index : public QObject{
  Q_OBJECT

public:
	Index(QObject *p = 0);
	virtual ~Index();
	void init();
	void save(Doc&);
	void deleteDoc(Doc&);
	void deleteDoc(const QString&);

private:
	QScopedPointer<Xapian::WritableDatabase> db;

};

#endif /* APP_INDEX_H_ */
