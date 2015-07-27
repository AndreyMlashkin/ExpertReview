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
        mainwindow.cpp \
    treepropertywidget.cpp \
    treemetainfo.cpp \
    properynode.cpp

HEADERS  += mainwindow.h \
    treepropertywidget.h \
    treemetainfo.h \
    properynode.h

FORMS    += mainwindow.ui

LIBS += -L$$PWD/../bin \
        -lqtpropertybrowser

RESOURCES += \
    resources.qrc