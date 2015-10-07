#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T23:42:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExpertReview
TEMPLATE = app
INCLUDEPATH += ../include/qtpropertybrowser

DESTDIR = $$PWD/../bin
OBJECTS_DIR = ./tmp/obj
UI_DIR = ./tmp/ui
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

SOURCES += main.cpp\
    treepropertywidget.cpp \
    properynode.cpp \
    propertytreeviewer.cpp \
    modechooser.cpp \
    projectsourcedata.cpp \
    nodesinfo/treeleftsideinfofile.cpp \
    nodesinfo/treeleftsideinfotest.cpp \
    nodesinfo/treerightsidevaluesfile.cpp

HEADERS  += \
    treepropertywidget.h \
    properynode.h \
    propertytreeviewer.h \
    modechooser.h \
    projectsourcedata.h \
    nodesinfo/treeleftsideinfo.h \
    nodesinfo/treeleftsideinfofile.h \
    nodesinfo/treeleftsideinfotest.h \
    nodesinfo/treerightsidevalues.h \
    nodesinfo/treerightsidevaluesfile.h

FORMS    += \
    propertytreeviewer.ui \
    modechooser.ui \
    projectsourcedata.ui

LIBS += -L$$PWD/../bin \
        -lqtpropertybrowser

RESOURCES += \
    resources.qrc
