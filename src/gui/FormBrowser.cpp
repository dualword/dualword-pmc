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

#include "FormBrowser.h"
#include "Browser.h"

#include <QtGui>

FormBrowser::FormBrowser(QWidget *p) : Form(p), browser(new Browser(this)) {
	QVBoxLayout *box = new QVBoxLayout(this);
	box->addWidget(browser);
	setLayout(box);

    QObject::connect(browser,SIGNAL(titleChanged(const QString&)),
    		SIGNAL(titleChanged(const QString&)));
    QObject::connect(browser->page(), SIGNAL(linkHovered(const QString&, const QString&,
    		const QString&)), SIGNAL(statusBarMessage(const QString&)));

    browser->setUrl(QUrl("http://www.ncbi.nlm.nih.gov/pmc/"));

}

FormBrowser::~FormBrowser() {

}

QString FormBrowser::getTitle() const{
	return browser->page()->currentFrame()->title();
}


