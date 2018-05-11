include("dualword-pmc.version.pri")

TARGET = dualword-pmc
TEMPLATE = app
QT += xml xmlpatterns
 
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += qt webkit network release

message(Building: = $${TARGET} - $${VERSION})

HEADERS += \
	app/DualwordPmcApp.h \
	app/NetworkAccessManager.h \
	gui/Form.h \
	gui/Tab.h \
	gui/MainWindow.h \
	gui/Browser.h \
	gui/FormBrowser.h
			
SOURCES += \
	app/main.cpp \
	app/DualwordPmcApp.cpp \
	app/NetworkAccessManager.cpp \
	gui/slots.cpp \
	gui/Form.cpp \
	gui/Tab.cpp \
	gui/MainWindow.cpp \
	gui/Browser.cpp \
	gui/FormBrowser.cpp

FORMS += \
	gui/MainWindow.ui
	
	
unix {	
	include("dualword-pmc.unix.pri")
}

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui
