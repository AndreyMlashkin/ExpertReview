#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T23:42:55
#
#-------------------------------------------------

MODULENAME = expertworksheet
include(../common.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = expertworksheet
TEMPLATE = app

INCLUDEPATH += ../expertreviewlib

SOURCES += main.cpp

LIBS += -L$$PWD/../bin         \
        -lexpertreview         \
        -lqtpropertybrowser    \
        -lstatisticsview       \
        -lexpressioncalculator
