TARGET = expressioncalculator
TEMPLATE = lib
DEFINES += EXPRESSIONCALCULATOR_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$PWD/../bin
OBJECTS_DIR = ./tmp/obj
UI_DIR = ./tmp/ui
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

SOURCES += \
    error.cpp \
    functions.cpp \
    parser.cpp \
    variablelist.cpp \
    parseradaptor.cpp

HEADERS  += \
    constants.h \
    error.h \
    functions.h \
    parser.h \
    variablelist.h \
    parseradaptor.h \
    export.h

