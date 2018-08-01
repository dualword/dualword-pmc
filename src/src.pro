include("dualword-pmc.version.pri")

TARGET = dualword-pmc
TEMPLATE = app

QT += sql network webkit xml xmlpatterns
 
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += qt webkit network thread release

message(Building: = $${TARGET} - $${VERSION})

HEADERS += \
	app/Doc.h \
	app/DualwordPmcApp.h \
	app/NetworkAccessManager.h \
	app/FormDbConnection.h \
	app/Index.h \
	app/Indexer.h \
	app/IndexModel.h \
	gui/TableView.h \
	gui/GetPdf.h \
	gui/Form.h \
	gui/Tab.h \
	gui/MainWindow.h \
	gui/Browser.h \
	gui/FormBrowser.h \
	gui/FormTable.h \
	gui/FormViewer.h
			
SOURCES += \
	app/main.cpp \
	app/Doc.cpp \
	app/DualwordPmcApp.cpp \
	app/NetworkAccessManager.cpp \
	app/FormDbConnection.cpp \
	app/Index.cpp \
	app/Indexer.cpp \
	app/IndexModel.cpp \
	gui/TableView.cpp \
	gui/slots.cpp \
	gui/Form.cpp \
	gui/Tab.cpp \
	gui/MainWindow.cpp \
	gui/Browser.cpp \
	gui/FormBrowser.cpp \
	gui/FormTable.cpp \
	gui/FormViewer.cpp

FORMS += \
	gui/MainWindow.ui
	
RESOURCES += html/html.qrc

unix {	
	include("dualword-pmc.unix.pri")
}

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui
