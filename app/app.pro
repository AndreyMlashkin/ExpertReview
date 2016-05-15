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
    serialization/nodesinfo/propertynode.cpp \
    projectcalculation.cpp \
    finalcalculationdialog.cpp \
    serialization/nodesinfo/treeleftsideinfo.cpp \
    serialization/nodesinfo/treeleftsideinfofactory.cpp \
    serialization/jsonserialiser/treeleftsideinfojson.cpp \
    serialization/jsonserialiser/propertynodejson.cpp \
    serialization/jsonserialiser/treerightsidevaluesjson.cpp \
    fulltreetablemodel.cpp \
    finalreportviewdelegate.cpp \
    projectchoosedialog.cpp \

HEADERS  += \
    treepropertywidget.h \
    propertytreeviewer.h \
    modechooser.h \
    serialization/nodesinfo/propertynode.h \
    serialization/nodesinfo/treeleftsideinfo.h \
    serialization/nodesinfo/treerightsidevalues.h \
    projectcalculation.h \
    finalcalculationdialog.h \
    projectapi.h \
    serialization/nodesinfo/treeleftsideinfofactory.h \
    serialization/jsonserialiser/treeleftsideinfojson.h \
    serialization/jsonserialiser/propertynodejson.h \
    serialization/jsonserialiser/treerightsidevaluesjson.h \
    fulltreetablemodel.h \
    finalreportviewdelegate.h \
    projectchoosedialog.h \

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
