#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T23:42:55
#
#-------------------------------------------------

include(../common.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExpertReview
TEMPLATE = app

INCLUDEPATH += .. \
               nodesinfo \
               ../expressioncalculator \
               ../statisticsview

SOURCES += main.cpp\
    treepropertywidget.cpp \
    propertytreeviewer.cpp \
    modechooser.cpp \
    serialization/nodesinfo/propertynode.cpp \
    projectcalculation.cpp \
    finalcalculationdialog.cpp \
    serialization/nodesinfo/treeleftsideinfo.cpp \
    serialization/jsonserialiser/treeleftsideinfojson.cpp \
    serialization/jsonserialiser/propertynodejson.cpp \
    serialization/jsonserialiser/treerightsidevaluesjson.cpp \
    fulltreetablemodel.cpp \
    finalreportviewdelegate.cpp \
    serialization/projectsloader.cpp \
    projectchooser.cpp \
    rightsidediagrammview.cpp \
    simplystatistic.cpp

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
    serialization/jsonserialiser/treeleftsideinfojson.h \
    serialization/jsonserialiser/propertynodejson.h \
    serialization/jsonserialiser/treerightsidevaluesjson.h \
    fulltreetablemodel.h \
    finalreportviewdelegate.h \
    serialization/projectsloader.h \
    projectchooser.h \
    rightsidediagrammview.h \
    simplystatistic.h

FORMS    += \
    propertytreeviewer.ui \
    modechooser.ui \
    finalcalculationdialog.ui \
    projectchooser.ui

LIBS += -L$$PWD/../bin         \
        -lqtpropertybrowser    \
        -lstatisticsview       \
        -lexpressioncalculator

RESOURCES += \
    resources.qrc
