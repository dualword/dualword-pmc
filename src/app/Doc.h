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

#ifndef DOC_H_
#define DOC_H_

#include <QtCore>
#include <QImage>

#include "public/fpdfview.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_text.h"

class Doc : public QObject{
	Q_OBJECT

public:
	Doc(QObject *p = 0);
	~Doc();
	void open();
	void toText(QString &);

signals:
	void newPage(const QImage*);
	void newImage(const QImage*);
	void newDoc();

public slots:
	void loadPage(int);
	void getImages();

	void setZoom(int z) {
		zoom = z;
		loadPage(++pageNum);
	}

	int getPageCount() const {
		return pageCount;
	}

	int getImageCount() const {
		return imageCount;
	}

	const QByteArray& getData() const {
		return data;
	}

	void setData(const QByteArray& data) {
		this->data = data;
	}

	const QString& getName() const {
		return name;
	}

	void setName(const QString& name) {
		this->name = name;
	}

	const QString& getPmcid() const {
		return pmcid;
	}

	void setPmcid(const QString& pmcid) {
		this->pmcid = pmcid;
	}

	int getSize() const {
		return size;
	}

	void setSize(int size) {
		this->size = size;
	}

	void emitNewDoc() { emit newDoc(); }

	const qint64& getCreated() const {
		return created;
	}

	void setCreated(const qint64& created) {
		this->created = created;
	}

private:
	Q_DISABLE_COPY(Doc)
	void rgba(QImage&, unsigned char*, int, int);

	FPDF_DOCUMENT doc;
	int pageNum, pageCount, imageCount;
	float zoom, res;
	int size;
	QString name;
	QString pmcid;
	QByteArray data;
	qint64 created = 0;

};

inline QDebug operator<<(QDebug dbg, const Doc *doc){
	dbg.nospace() << "Doc: pages:" << doc->getPageCount() << " images:" << doc->getImageCount();
    return dbg.maybeSpace();
}

#endif /* DOC_H_ */
