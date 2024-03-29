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

#ifndef FORMVIEWER_H_
#define FORMVIEWER_H_

#include "Form.h"

class Doc;
class FormDbConnection;

class FormViewer : public Form {
  Q_OBJECT

public:
	FormViewer(QWidget *p = 0);
	virtual ~FormViewer();
	virtual QString getTitle() const;
	void init();

public slots:
	void setPage(const QImage*);
	void setImage(const QImage*);
	void loadDoc(const QString& i);
	void contextMenuRequested(const QPoint&);

	bool loadImages() const {
		return loadImage;
	}

	void setLoadImages(bool loadImage) {
		this->loadImage = loadImage;
		list->setVisible(loadImage);
	}

private slots:
	void createUi();
	void connectSlots();
	void copyDoc();
	void clear();

private:
	QPushButton *btnBack;
	QLabel *lbl, *page;
	QScrollArea *scroll;
	QScopedPointer<Doc> pdf;
	FormDbConnection *db;
	QSpinBox *slideZ, *slideP;
	QString name;
	QListWidget* list;
	bool loadImage;

};

#endif /* FORMVIEWER_H_ */
