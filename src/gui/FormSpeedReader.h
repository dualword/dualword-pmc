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

#ifndef GUI_FORMSPEEDREADER_H_
#define GUI_FORMSPEEDREADER_H_

#include "Form.h"

class FormSpeedReader : public MainForm<FormSpeedReader> {
  Q_OBJECT

public:
	FormSpeedReader(QWidget *p = 0);
	virtual ~FormSpeedReader();
	virtual QString getTitle() const;
	virtual void init();

public slots:
	void contextMenuRequested(const QPoint&);
	void loadDoc(const QString& i);

private slots:
	void createUi();
	void connectSlots();
	void start();
	void update();

private:
	QLabel* page;
	QSpinBox *slideW, *slideS;
	QPushButton *btnStart;
	QFont font;
	QTimer timer;
	QString txt;
	QStringList words;
	long idx;
	QString name;
};

#endif /* GUI_FORMSPEEDREADER_H_ */
