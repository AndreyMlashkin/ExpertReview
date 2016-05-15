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
    nodesinfo/propertynode.cpp \
    projectcalculation.cpp \
    finalcalculationdialog.cpp \
    nodesinfo/treeleftsideinfo.cpp \
    nodesinfo/treeleftsideinfofactory.cpp \
    jsonserialiser/treeleftsideinfojson.cpp \
    jsonserialiser/propertynodejson.cpp \
    jsonserialiser/treerightsidevaluesjson.cpp \
    fulltreetablemodel.cpp \
    finalreportviewdelegate.cpp \
    projectchoosedialog.cpp

HEADERS  += \
    treepropertywidget.h \
    propertytreeviewer.h \
    modechooser.h \
    nodesinfo/propertynode.h \
    nodesinfo/treeleftsideinfo.h \
    nodesinfo/treerightsidevalues.h \
    projectcalculation.h \
    finalcalculationdialog.h \
    projectapi.h \
    nodesinfo/treeleftsideinfofactory.h \
    jsonserialiser/treeleftsideinfojson.h \
    jsonserialiser/propertynodejson.h \
    jsonserialiser/treerightsidevaluesjson.h \
    fulltreetablemodel.h \
    finalreportviewdelegate.h \
    projectchoosedialog.h

FORMS    += \
    propertytreeviewer.ui \
    modechooser.ui \
    finalcalculationdialog.ui \
    projectchoosedialog.ui

LIBS += -L$$PWD/../bin \
        -lqtpropertybrowser \
        -lexpressioncalculator

RESOURCES += \
    resources.qrc
