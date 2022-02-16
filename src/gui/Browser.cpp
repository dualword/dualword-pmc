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

#include "Browser.h"
#include "app/global.h"
#include "app/NetworkAccessManager.h"
#include "gui/GetPdf.h"

WebPage::WebPage(QObject *p) : QWebEnginePage(p){

}

bool WebPage::acceptNavigationRequest(const QUrl& url, QWebEnginePage::NavigationType type, bool mframe){
	if (type == QWebEnginePage::NavigationTypeLinkClicked) {
		if(!isValidUrl(url)) return false;
	}
    return QWebEnginePage::acceptNavigationRequest(url, type, mframe);
}

bool WebPage::isValidUrl(const QUrl& url){
	return url.host().toLower().endsWith("ncbi.nlm.nih.gov");
}

WebPage::~WebPage() {
	history()->clear();
}

Browser::Browser(QWidget *p) : QWebEngineView(p), searchString(""){
	setPage(new WebPage(this));
	new QShortcut(QKeySequence::Find, this, SLOT(findTxt()), nullptr, Qt::WidgetWithChildrenShortcut);
    QObject::connect(page(), SIGNAL(loadFinished(bool)), SLOT(loadFinished(bool)));
}

Browser::~Browser() {

}

void Browser::findTxt(){
    bool ok;
    QString search = QInputDialog::getText(this, tr("Find"), tr("Text:"), QLineEdit::Normal,
    		searchString, &ok);
    if (ok) {
    	searchString = search;
        findText(searchString);
    }
}

void Browser::contextMenuEvent(QContextMenuEvent *event){
    auto r = page()->contextMenuData().linkUrl();
    if (!r.isEmpty()) {
        QMenu menu(this);
        if((qobject_cast<WebPage*>(page()))->isValidUrl(r)){
        	QAction *a = menu.addAction(tr("Open Link in New Tab"), [&]{
    			mainWin->getTab()->createBrowser(a->data().toUrl());});
    		a->setData(r);
    		menu.addSeparator();
        }
        menu.addAction(pageAction(QWebEnginePage::CopyLinkToClipboard));
        menu.exec(mapToGlobal(event->pos()));
        return;
    }
    QWebEngineView::contextMenuEvent(event);
}

void Browser::loadFinished (bool ok){
	if(url().toString().contains("ncbi.nlm.nih.gov/pmc/articles",Qt::CaseInsensitive)){
		 page()->runJavaScript("document.querySelector('link[rel=\"alternate\"][type=\"application/pdf\"]').getAttribute('href')",
				 [=](QVariant var){
			 	 	 QString link = var.toString().trimmed();
			 	 	 if( link.length() > 0)
			 	 		 pmcApp->startTask<DownloadTask>("https://www.ncbi.nlm.nih.gov" + link);
			});
	}
}

