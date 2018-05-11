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

#ifndef BROWSER_H_
#define BROWSER_H_

#include <QtCore>
#include <QtWebKit>

class WebPage : public QWebPage{
	Q_OBJECT

public:
	WebPage(QObject *p = 0);
	virtual ~WebPage();

protected:
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type);

};

class Browser : public QWebView {
	Q_OBJECT

public:
	Browser(QWidget *p = 0);
	virtual ~Browser();
	int getId(){return id;};

public slots:
	void findTxt();

protected slots:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual	bool isValidUrl(const QUrl&);
	void openLink();

protected:
	QString searchString;
	int id;

};


#endif /* BROWSER_H_ */
