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

#include "FormViewer.h"
#include "app/global.h"

FormViewer::FormViewer(QWidget *p) : Form(p), pdf(0), loadImage(true) {
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
    page->setAlignment(Qt::AlignHCenter);
	scroll = new QScrollArea(this);
	scroll->setAlignment(Qt::AlignCenter);
	scroll->setBackgroundRole(QPalette::Dark);
	scroll->setWidget(lbl);
	slideZ = new QSpinBox(this);
	slideZ->setToolTip("Zoom");
	slideZ->setMinimum(50);
	slideZ->setMaximum(200);
	slideZ->setValue(pmcApp->value("zoom", 120).toInt());
	slideZ->setSingleStep(5);
	slideZ->setAlignment(Qt::AlignHCenter);
	slideP = new QSpinBox(this);
	slideP->setToolTip("Page");
	slideP->setMinimum(0);
	slideP->setMaximum(0);
	slideP->setValue(0);
	slideP->setSingleStep(0);
	slideP->setWrapping(true);
	slideP->setAlignment(Qt::AlignHCenter);

	QVBoxLayout *bl = new QVBoxLayout();
	bl->addStretch(0);
	bl->setSpacing(0);
	bl->setContentsMargins(0,0,0,0);
	bl->addWidget(slideZ);
	bl->addWidget(slideP);
	bl->addWidget(page);

	list = new QListWidget(this);
	list->setViewMode(QListWidget::IconMode);
	list->setIconSize(QSize(250,250));
	list->setResizeMode(QListWidget::Adjust);
	list->setFlow(QListView::TopToBottom);
	list->setWrapping(false);

	QHBoxLayout *layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(scroll, 1);
	layout->addLayout(bl, 0);

	QWidget* w = new QWidget(this);
	w->setLayout(layout);

	QSizePolicy p = w->sizePolicy();
	p.setHorizontalStretch(1);
	w->setSizePolicy(p);

	QSplitter* sp = new QSplitter(this);
	sp->addWidget(list);
	sp->addWidget(w);

	QVBoxLayout *layout1 = new QVBoxLayout(this);
	layout1->setSpacing(0);
	layout1->setContentsMargins(0,0,0,0);
	layout1->addWidget(sp);

    setLayout(layout1);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),
			SLOT(contextMenuRequested(const QPoint&)));
}

void FormViewer::connectSlots(){

}

void FormViewer::contextMenuRequested(const QPoint& p){
	QMenu menu(this);
	menu.addAction(tr("Copy to Clipboard"), this, SLOT(copyDoc()));
	menu.exec(QCursor::pos());
}

void FormViewer::copyDoc(){
	if(pdf.isNull()) return;
	QString s;
	pdf->toText(s);
	QApplication::clipboard()->setText(s);
}

void FormViewer::setPage(const QImage* image){
	lbl->setPixmap(QPixmap::fromImage(*image));
	lbl->adjustSize();
}

void FormViewer::setImage(const QImage* image){
	list->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(*image)), 0, list, 0));
}

void FormViewer::loadDoc(const QString& i){
	clear();
	if(i == "-1") return;
	try {
		pdf.reset(new Doc());
		db->getDoc(*pdf.data(),i);
		if(pdf->getPmcid().length() <= 0) return;
		pdf->open();
		name = pdf->getName();
		connect(pdf.data(), SIGNAL(newPage(const QImage*)), this, SLOT(setPage(const QImage*)));
		connect(slideZ, SIGNAL(valueChanged(int)), pdf.data(), SLOT(setZoom(int)));
		connect(slideZ, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i) {
			pmcApp->setValue("zoom", i);
		});
		pdf->setZoom(slideZ->value());
		slideP->setEnabled(true);
		slideP->setMinimum(1);
		slideP->setMaximum(pdf->getPageCount());
		slideP->setValue(1);
		slideP->setSingleStep(1);
		connect(slideP, SIGNAL(valueChanged(int)), pdf.data(), SLOT(loadPage(int)));
		page->setText("Pages:"+QString::number(pdf->getPageCount()));
		if(loadImage){
			connect(pdf.data(), SIGNAL(newImage(const QImage*)), this, SLOT(setImage(const QImage*)));
			pdf->getImages();
		}
	} catch (const dualword_exception& e) {
		//
	}
}

QString FormViewer::getTitle() const{
	return "";
}

void FormViewer::clear(){
	lbl->clear();
	list->clear();
	pdf.reset(0);
	page->setText("");
	slideP->setMinimum(0);
	slideP->setValue(0);
	slideP->setEnabled(false);
}
