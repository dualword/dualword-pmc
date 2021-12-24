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

WebPage::WebPage(QObject *p) : QWebPage(p){
	setNetworkAccessManager(new NetworkAccessManager(this));
	history()->setMaximumItemCount(25);
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type){
	if (type == QWebPage::NavigationTypeLinkClicked) {
		if(!request.url().host().toLower().contains("ncbi.nlm.nih.gov")) return false;
	}
    return QWebPage::acceptNavigationRequest(frame, request, type);
}

WebPage::~WebPage() {

}

Browser::Browser(QWidget *p) : QWebView(p), searchString(""){
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
    auto r = page()->mainFrame()->hitTestContent(event->pos());
    if (!r.linkUrl().isEmpty()) {
        QMenu menu(this);
        if(isValidUrl(r.linkUrl())){
    		auto a = menu.addAction(tr("Open Link in New Tab"), this, SLOT(openLink()));
    		a->setData(r.linkUrl());
    		menu.addSeparator();
        }
        menu.addAction(pageAction(QWebPage::CopyLinkToClipboard));
        menu.exec(mapToGlobal(event->pos()));
        return;
    }
    QWebView::contextMenuEvent(event);
}

void Browser::openLink(){
	auto a = qobject_cast<QAction*>(sender());
	mainWin->getTab()->createBrowser(a->data().toUrl());
}

bool Browser::isValidUrl(const QUrl& url){
	return url.host().contains("ncbi.nlm.nih.gov",Qt::CaseInsensitive);
}

void Browser::loadFinished (bool ok){
	if(!ok){
		setHtml((pmcApp->getHtml(":/error.html")).arg("").arg(url().toString()));
		emit titleChanged("Error");
		return;
	}

	if(url().toString().contains("ncbi.nlm.nih.gov/pmc/articles",Qt::CaseInsensitive)){
		QWebFrame *frame = page()->mainFrame();
		if(!frame) return;
		QWebElement doc = frame->documentElement();
		if(doc.isNull()) return;
		QString link = doc.findFirst("link[rel=\"alternate\"][type=\"application/pdf\"]").attribute("href");
		if(link.length()>0) pmcApp->startTask<DownloadTask>("https://www.ncbi.nlm.nih.gov"+link);
	}
}
