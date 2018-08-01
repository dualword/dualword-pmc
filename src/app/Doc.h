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

#ifndef DOC_H_
#define DOC_H_

#include <QtCore>
#include <QImage>

extern "C"{
	#include <mupdf/pdf.h>
	#include <mupdf/fitz.h>
}

class Doc : public QObject{
	Q_OBJECT

public:
	Doc(QObject *p = 0);
	~Doc();
	void open();
	void toText(QString &);

public slots:
	void loadPage(int);
	void setZoom(int z) {
		zoom = z;
		loadPage(++pageNum);
	}
	int getPageCount() const {
		return pageCount;
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

Q_SIGNALS:
	void newImage(const QImage*);

private:
	Q_DISABLE_COPY(Doc)

	fz_context *ctx;
	fz_document *doc;
	int zoom,pageNum,pageCount;
	int size;
	QString name;
	QString pmcid;
	QByteArray data;

};

inline QDebug operator<<(QDebug dbg, const Doc *doc){
	dbg.nospace() << "Doc: pages:" << doc->getPageCount() ;
    return dbg.maybeSpace();
}

#endif /* DOC_H_ */
