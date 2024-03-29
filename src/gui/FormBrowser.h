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

#ifndef GUI_FORMBROWSER_H_
#define GUI_FORMBROWSER_H_

#include "Form.h"

class Browser;

class FormBrowser : public MainForm<FormBrowser> {
	Q_OBJECT

public:
	FormBrowser(QWidget *p = 0);
	virtual ~FormBrowser();
	virtual QString getTitle() const;
	Browser* getBrowser() const { return browser; };

private:
	Browser* browser;
};

#endif /* GUI_FORMBROWSER_H_ */
