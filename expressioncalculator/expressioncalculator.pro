include(../common.pri)

TARGET = expressioncalculator
TEMPLATE = lib
DEFINES += EXPRESSIONCALCULATOR_LIBRARY

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

