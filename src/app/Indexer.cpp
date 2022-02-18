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

#include "Index.h"
#include "Indexer.h"
#include "global.h"

Indexer::Indexer(QObject *p) : QThread(p), stop(false){

}

Indexer::~Indexer() {

}

void Indexer::run(){
	FormDbConnection db;
	try {
		db.open();
		while(!stop){
			QScopedPointer<Doc> doc(new Doc());
			if (!db.getNextDelete(*doc.get())) break;
			emit newMsg("Deleting " + doc->getPmcid()) ;
			QObject::connect(doc.get(), SIGNAL(newDoc()), mainWin->getTab(), SLOT(indexChange()), Qt::QueuedConnection);
			pmcApp->index()->deleteDoc(doc->getPmcid());
			db.deleteDoc(doc->getPmcid());
			doc->emitNewDoc();
		}
		while(!stop){
			QScopedPointer<Doc> doc(new Doc());
			if (!db.getNextDoc(*doc.get())) break;
			emit newMsg("Indexing " + doc->getPmcid());
			QObject::connect(doc.get(), SIGNAL(newDoc()), mainWin->getTab(), SLOT(indexChange()), Qt::QueuedConnection);
			doc->open();
			doc->getImages();
			pmcApp->index()->save(*doc.get());
			db.updateDoc(doc->getPmcid());
			doc->emitNewDoc();
		}
		emit newMsg("");
	} catch(const dualword_exception& e) {
		//
	}
}
