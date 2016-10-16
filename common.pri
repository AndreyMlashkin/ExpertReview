QMAKE_CXXFLAGS += -std=c++11

debug {
    DESTDIR     = $$PWD/bin
    OBJECTS_DIR = $$PWD/build/$$MODULENAME/debug/obj
    UI_DIR      = $$PWD/build/$$MODULENAME/debug/ui
    MOC_DIR     = $$PWD/build/$$MODULENAME/debug/moc
    RCC_DIR     = $$PWD/build/$$MODULENAME/debug/rcc
}
else {
    DESTDIR     = $$PWD/bin
    OBJECTS_DIR = $$PWD/build/$$MODULENAME/release/obj
    UI_DIR      = $$PWD/build/$$MODULENAME/release/ui
    MOC_DIR     = $$PWD/build/$$MODULENAME/release/moc
    RCC_DIR     = $$PWD/build/$$MODULENAME/release/rcc
}

