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

#ifndef GUI_GETPDF_H_
#define GUI_GETPDF_H_

#include <QtNetwork>
#include "app/global.h"
#include "app/NetworkAccessManager.h"

class GetPdf : public QObject{
  Q_OBJECT

public:
  GetPdf(const QString& s, QObject* p = 0): QObject(p), link(s){};
	virtual ~GetPdf(){};
	void get(){
		FormDbConnection db(this);
		try {
			db.open();
			if(db.exists(link)) return;
			QUrl api = QUrl(link);
			NetworkAccessManager *networkMgr = new NetworkAccessManager(this);
			QNetworkReply *reply = networkMgr->get(QNetworkRequest(api));
			QEventLoop loop;
			QObject::connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
			QObject::connect(reply,SIGNAL(finished()),reply,SLOT(deleteLater()));
			QObject::connect(reply,SIGNAL(finished()),SLOT(deleteLater()));
			loop.exec();
			QByteArray arr = QByteArray(reply->readAll());
			if (reply->error() != QNetworkReply::NoError) return;
			Doc d;
			d.setName(link.mid(link.lastIndexOf("/")+1));
			d.setSize(arr.size());
			d.setData(arr);
			d.open();
			QRegExp rx("/(PMC[\\d]+)/");
			if(rx.indexIn(link) == -1) return;
			d.setPmcid(rx.cap(1));
			db.saveDoc(d);
			emit start();
		} catch(const dualword_exception& e) {
			//
		}
	}

signals:
	void start();

private:
	QString link;
};

class DownloadTask : public QRunnable {
public:
	DownloadTask(const QString& s) : link(s) {};
	virtual ~DownloadTask() { };
	void run() {
		GetPdf get(link);
	    QObject::connect(&get, SIGNAL(start()), pmcApp,SLOT(startIndexer()));
		get.get();
	}
private:
     QString link;

};


#endif /* GUI_GETPDF_H_ */
