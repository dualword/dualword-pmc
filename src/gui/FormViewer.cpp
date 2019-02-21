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

#include "FormViewer.h"
#include "app/FormDbConnection.h"
#include "app/Doc.h"
#include "app/global.h"

FormViewer::FormViewer(QWidget *p) : Form(p) {
	createUi();

}

FormViewer::~FormViewer() {

}

void FormViewer::init(){
	db = new FormDbConnection(this);
	try {
		db->open();
	} catch(const dualword_exception& e) {
		//QMessageBox::critical(this, "err", QString::fromStdString(e.what()));
	}
}

void FormViewer::createUi(){
	lbl = new QLabel(this);
    lbl->setBackgroundRole(QPalette::Base);
    lbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    lbl->setScaledContents(true);
    page = new QLabel(this);
	scroll = new QScrollArea(this);
	scroll->setAlignment(Qt::AlignCenter);
	scroll->setBackgroundRole(QPalette::Dark);
	scroll->setWidget(lbl);
	slideZ = new QSpinBox(this);
	slideZ->setToolTip("Zoom");
	slideZ->setMinimum(50);
	slideZ->setMaximum(200);
	slideZ->setValue(150);
	slideZ->setSingleStep(5);
	slideP = new QSpinBox(this);
	slideP->setToolTip("Page");
	slideP->setMinimum(0);
	slideP->setMaximum(0);
	slideP->setValue(0);
	slideP->setSingleStep(0);
	slideP->setWrapping(true);
	QHBoxLayout *bl = new QHBoxLayout();
	bl->addStretch(0);
	bl->addWidget(slideZ);
	bl->addWidget(slideP);
	bl->addWidget(page);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(scroll);
	layout->addLayout(bl);
    setLayout(layout);
}

void FormViewer::connectSlots(){

}


void FormViewer::setImage(const QImage* image){
	lbl->setPixmap(QPixmap::fromImage(*image));
	lbl->adjustSize();
}

void FormViewer::loadDoc(const QString& i){
	pdf.reset(new Doc());
	try {
		db->getDoc(*pdf.data(),i);
		name = pdf->getName();
		connect(pdf.data(), SIGNAL(newImage(const QImage*)), this, SLOT(setImage(const QImage*)));
		connect(slideZ, SIGNAL(valueChanged(int)), pdf.data(), SLOT(setZoom(int)));
		pdf->setZoom(slideZ->value());
		slideP->setMinimum(1);
		slideP->setMaximum(pdf->getPageCount());
		slideP->setValue(1);
		slideP->setSingleStep(1);
		connect(slideP, SIGNAL(valueChanged(int)), pdf.data(), SLOT(loadPage(int)));
		page->setText("/"+QString::number(pdf->getPageCount()));
	} catch (const dualword_exception& e) {
		//
	}
}

QString FormViewer::getTitle() const{
	return "";
}
