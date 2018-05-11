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

#ifndef SRC_APP_NETWORKACCESSMANAGER_H_
#define SRC_APP_NETWORKACCESSMANAGER_H_

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>

class NetworkCookieJar : public QNetworkCookieJar{
	Q_OBJECT

public:
	NetworkCookieJar (QObject *p = 0);
	virtual ~NetworkCookieJar ();
	int count();

};

class NetworkAccessManager : public QNetworkAccessManager{
	Q_OBJECT

public:
	NetworkAccessManager(QObject *p = 0);
	virtual ~NetworkAccessManager();

};

#endif /* SRC_APP_NETWORKACCESSMANAGER_H_ */
