QMAKE_CXXFLAGS += $$CFLAGS $$CPPFLAGS $$CXXFLAGS
QMAKE_LFLAGS += $$LDFLAGS

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -I/usr/include/qt4/QtWebKit -I/usr/include/qt4/QtNetwork
QMAKE_CXXFLAGS += -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-deprecated

LIBS += -lQtWebKit -lQtNetwork
