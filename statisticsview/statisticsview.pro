#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T22:25:20
#
#-------------------------------------------------

include(../common.pri)

TARGET = statisticsview
TEMPLATE = lib
DEFINES += STATISTIC_VIEW_LIBRARY
QT       += widgets

SOURCES += statisticswidget.cpp \
           statistic.cpp \
           pieview.cpp

HEADERS += export.h \
           statisticswidget.h \
           statistic.h \
           pieview.h

FORMS   += statistic.ui

