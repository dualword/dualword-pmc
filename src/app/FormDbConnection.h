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

#ifndef FORMDBCONNECTION_H_
#define FORMDBCONNECTION_H_

#include <QtSql>

class Doc;
struct doc;

class FormDbConnection : public QObject{
	Q_OBJECT

public:
	FormDbConnection(QObject *p = 0);
	virtual ~FormDbConnection();
	void open();
	void create();
	QSqlDatabase connection() {return db;};
	void saveDoc(const Doc &d);
	void getDoc(Doc &d, const QString& id);
	bool getNextDoc(Doc &d);
	void deleteDoc(const QString&);
	void updateDoc(const QString&);
	bool exists(const QString&);
	void reindex();
	void clearHistory();
	void saveSearch(const QString&);
	QStringList searchHistory();
	int getCount(const QString&);
	bool getValue(const QString&, QString&);

private:
	void execSql(const QString&);

	QSqlDatabase db;
	QString name;

};

#endif /* FORMDBCONNECTION_H_ */
