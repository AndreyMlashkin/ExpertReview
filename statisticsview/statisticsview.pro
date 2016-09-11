#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T22:25:20
#
#-------------------------------------------------

TARGET = statisticsview
TEMPLATE = lib
DEFINES += STATISTIC_VIEW_LIBRARY
QT       += widgets

QMAKE_CXXFLAGS += -std=c++11

DESTDIR = $$PWD/../bin
OBJECTS_DIR = ./tmp/obj
UI_DIR = ./tmp/ui
MOC_DIR = ./tmp/moc
RCC_DIR = ./tmp/rcc

SOURCES += statisticswidget.cpp \
           statistic.cpp \
           pieview.cpp

HEADERS += export.h \
           statisticswidget.h \
           statistic.h \
           pieview.h

FORMS   += statistic.ui

