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

#include "gui/MainWindow.h"
#include "app/global.h"

void MainWindow::setSlots() {
    QObject::connect(actionExit,SIGNAL(triggered()), SLOT(exit()));
    QObject::connect(actionAbout,SIGNAL(triggered()), SLOT(showAbout()));
    QObject::connect(actionInfo,SIGNAL(triggered()), SLOT(showInfo()));
    QObject::connect(actionReindex,SIGNAL(triggered()), SLOT(reindex()));
    QObject::connect(actionStartIndexing,SIGNAL(triggered()), pmcApp, SLOT(startIndexer()));
    QObject::connect(actionStopIndexing,SIGNAL(triggered()), pmcApp, SLOT(stopIndexer()));
    QObject::connect(actionClearHistory,SIGNAL(triggered()), SLOT(clearHistory()));

}
