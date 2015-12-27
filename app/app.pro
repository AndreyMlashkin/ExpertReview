#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T23:42:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExpertReview
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += .. \
               /nodesinfo

DESTDIR = $$PWD/../bin
OBJECTS_DIR = ./tmp/obj
UI_DIR = ./tmp/ui
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

SOURCES += main.cpp\
    treepropertywidget.cpp \
    propertytreeviewer.cpp \
    modechooser.cpp \
    nodesinfo/treeleftsideinfofile.cpp \
    nodesinfo/treerightsidevaluesfile.cpp \
    projectcalculation.cpp \
    finalcalculationdialog.cpp \
    nodesinfo/treeleftsideinfo.cpp \
    nodesinfo/treeleftsideinfofactory.cpp \
    #jsonserialiser/treeleftsideinfojson.cpp \
    propertynode.cpp

HEADERS  += \
    treepropertywidget.h \
    propertytreeviewer.h \
    modechooser.h \
    nodesinfo/treeleftsideinfo.h \
    nodesinfo/treeleftsideinfofile.h \
    nodesinfo/treerightsidevalues.h \
    nodesinfo/treerightsidevaluesfile.h \
    projectcalculation.h \
    finalcalculationdialog.h \
    projectapi.h \
    nodesinfo/treeleftsideinfofactory.h \
    #jsonserialiser/treeleftsideinfojson.h \
    propertynode.h

FORMS    += \
    propertytreeviewer.ui \
    modechooser.ui \
    finalcalculationdialog.ui

LIBS += -L$$PWD/../bin \
        -lqtpropertybrowser \
        -lexpressioncalculator

RESOURCES += \
    resources.qrc
