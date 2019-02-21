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

#include "Index.h"
#include "Indexer.h"
#include "app/FormDbConnection.h"
#include "app/Doc.h"

#include "global.h"

Indexer::Indexer(QObject *p) : QThread(p){

}

Indexer::~Indexer() {

}

void Indexer::run(){
	FormDbConnection db;
	try {
		db.open();
		while(true){
			QScopedPointer<Doc> doc(new Doc());
			QObject::connect(doc.data(), SIGNAL(newDoc()), mainWin->getTab(), SLOT(indexChange()), Qt::QueuedConnection);
			if (!db.getNextDoc(*doc.data())) break;
			emit newMsg("Indexing " + doc->getPmcid());
			pmcApp->index()->save(*doc.data());
			db.updateDoc(doc->getPmcid());
			doc->emitNewDoc();
		}
		emit newMsg("");
	} catch(const dualword_exception& e) {
		//
	}
}
