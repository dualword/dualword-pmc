QMAKE_CXXFLAGS += $$CFLAGS $$CPPFLAGS $$CXXFLAGS
QMAKE_LFLAGS += $$LDFLAGS

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -I/usr/include/qt4/QtWebKit -I/usr/include/qt4/QtNetwork
QMAKE_CXXFLAGS += -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-deprecated
LIBS += -lQtWebKit -lQtNetwork -lxapian

MUPDF_DIR=/
INCLUDEPATH += $$MUPDF_DIR/include
QMAKE_CXXFLAGS += -I$$MUPDF_DIR/include
QMAKE_LFLAGS += -L$$MUPDF_DIR/build/debug/
LIBS += $$MUPDF_DIR/build/debug/libmupdf.a
#LIBS += $$MUPDF_DIR/build/debug/libfreetype.a
LIBS += $$MUPDF_DIR/build/debug/libjbig2dec.a
LIBS += $$MUPDF_DIR/build/debug/libjpeg.a
LIBS += $$MUPDF_DIR/build/debug/libopenjpeg.a
LIBS += $$MUPDF_DIR/build/debug/libmujs.a
LIBS += $$MUPDF_DIR/build/debug/libz.a
LIBS +=  -lm -lssl