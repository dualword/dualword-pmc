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

#include "FormSpeedReader.h"
#include "app/global.h"

FormSpeedReader::FormSpeedReader(QWidget *p) : MainForm(p), idx(0) {
	createUi();
	connectSlots();
}

FormSpeedReader::~FormSpeedReader() {

}

QString FormSpeedReader::getTitle() const{
	return QString("Reader: ").append(name);
}

void FormSpeedReader::init(){
	timer.setInterval(pmcApp->value("wpm", 500).toInt());
}

void FormSpeedReader::loadDoc(const QString& i){
	if(i == "-1") return;
	try {
		FormDbConnection db;
		Doc pdf;
		db.open()->getDoc(pdf,i);
		if(pdf.getPmcid().length() <= 0) return;
		pdf.open();
		name = pdf.getName();
		pdf.toText(txt);
		btnW->click();
	} catch (const dualword_exception& e) {
		//
	}
}

void FormSpeedReader::contextMenuRequested(const QPoint& p){

}

void FormSpeedReader::createUi(){
    page = new QLabel(this);
    page->setAlignment(Qt::AlignCenter);
    font.setPointSize(pmcApp->value("fsize", 15).toInt());
    page->setFont(font);
    page->setStyleSheet("QLabel{background:gray;color:black;}");
    page->setWordWrap(true);
	slideW = new QSpinBox(this);
	slideW->setAlignment(Qt::AlignHCenter);
	slideS = new QSpinBox(this);
	slideS->setToolTip("Font size:15-100");
	slideS->setMinimum(15);
	slideS->setMaximum(100);
	slideS->setValue(pmcApp->value("fsize", 55).toInt());
	slideS->setSingleStep(5);
	slideS->setAlignment(Qt::AlignHCenter);
	btnStart = new QPushButton("Start", this);

	btnW = new QPushButton(this);
	btnW->setCheckable(true);
	btnW->setText("W");
	btnW->setToolTip("by Word");
	btnW->setChecked(true);
	btnS = new QPushButton(this);
	btnS->setCheckable(true);
	btnS->setText("S");
	btnS->setToolTip("by Sentence");
	bg = new QButtonGroup(this);
	bg->setExclusive(true);
	bg->addButton(btnW,0);
	bg->addButton(btnS,1);
	connect(bg, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int i){
		timer.stop();
		idx = 0;
		btnStart->setText("Start");
		page->setText("");
		if(i == 0){
			min = 100, max = 1000;
			slideW->setSingleStep(100);
			words = txt.replace(QRegularExpression("[\\n\\r]")," ").split(QRegExp("\\s+"));
		}else{
			min = 1000, max = 30000;
			slideW->setSingleStep(1000);
			words = txt.replace(QRegularExpression("[\\n\\r]")," ").split(QRegExp("\\.[\\s]+(?=[A-Z][a-z]+)"));
		}
		slideW->setMinimum(min);
		slideW->setMaximum(max);
		slideW->setToolTip("Pause: " + QString::number(min) + "-" + QString::number(max) + "(ms)"); //(words per minute)
		slideW->setValue(pmcApp->value("wpm", 500).toInt());
	});

	QVBoxLayout *bl = new QVBoxLayout();
	bl->addStretch(0);
	bl->setSpacing(0);
	bl->setContentsMargins(0,0,0,0);
	bl->addWidget(btnW);
	bl->addWidget(btnS);
	bl->addStretch(1);
	bl->addWidget(slideW);
	bl->addWidget(slideS);
	bl->addWidget(btnStart);

	QHBoxLayout *layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0,0,0,0);
	layout->addWidget(page, 1);
	layout->addLayout(bl, 0);
    setLayout(layout);

}

void FormSpeedReader::connectSlots(){
	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	connect(btnStart, &QPushButton::clicked, this, &FormSpeedReader::start);
	connect(slideW, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i) {
		timer.setInterval(i);
		pmcApp->setValue("wpm", i);
	});
	connect(slideS, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i) {
		font.setPointSize(i);
		page->setFont(font);
		pmcApp->setValue("fsize", i);
	});
}

void FormSpeedReader::start(){
	if(words.size() <= 0) return;
		if(timer.isActive()){
			timer.stop();
			idx = 0;
			btnStart->setText("Start");
		}else{
			page->setText(words.at(idx++));
			timer.start(slideW->value());
			btnStart->setText("Stop");
		}
}

void FormSpeedReader::update(){
	if(words.size() >= idx){
		page->setText(words.at(idx++));
	}else{
		timer.stop();
		idx = 0;
		btnStart->setText("Start");
	}
}
