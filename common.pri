QMAKE_CXXFLAGS += -std=c++11


debug {
    DESTDIR     = $$PWD/bin
    OBJECTS_DIR = $$PWD/build/debug/obj
    UI_DIR      = $$PWD/build/debug/ui
    MOC_DIR     = $$PWD/build/debug/moc
    RCC_DIR     = $$PWD/build/debug/rcc
}
else {
    DESTDIR     = $$PWD/bin
    OBJECTS_DIR = $$PWD/build/release/obj
    UI_DIR      = $$PWD/build/release/ui
    MOC_DIR     = $$PWD/build/release/moc
    RCC_DIR     = $$PWD/build/release/rcc
}

