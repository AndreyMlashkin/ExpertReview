include(../common.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT *= widgets
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
TEMPLATE = lib

qtpropertybrowser-uselib:!qtpropertybrowser-buildlib {
    LIBS += -L$$QTPROPERTYBROWSER_LIBDIR -l$$QTPROPERTYBROWSER_LIBNAME
} else {
    DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
    SOURCES += qtpropertybrowser.cpp \
            qtpropertymanager.cpp \
            qteditorfactory.cpp \
            qtvariantproperty.cpp \
            qttreepropertybrowser.cpp \
            qtbuttonpropertybrowser.cpp \
            qtgroupboxpropertybrowser.cpp \
            qtpropertybrowserutils.cpp
    HEADERS += qtpropertybrowser.h \
            qtpropertymanager.h \
            qteditorfactory.h \
            qtvariantproperty.h \
            qttreepropertybrowser.h \
            qtbuttonpropertybrowser.h \
            qtgroupboxpropertybrowser.h \
            qtpropertybrowserutils_p.h
    RESOURCES += qtpropertybrowser.qrc
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTPROPERTYBROWSER_EXPORT
    else:qtpropertybrowser-uselib:DEFINES += QT_QTPROPERTYBROWSER_IMPORT
}
