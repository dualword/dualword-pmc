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

#ifndef GUI_FORM_H_
#define GUI_FORM_H_

#include <QtGui>

class Index;

class Form : public QWidget {
     Q_OBJECT

public:
	Form(QWidget *p = 0);
	virtual ~Form();
	virtual QString getTitle() const = 0;

signals:
	void titleChanged (const QString&);
	void statusBarMessage (const QString&);

protected:
	Index* idx;

};



#endif /* GUI_FORM_H_ */
