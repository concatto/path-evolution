TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32 {
    INCLUDEPATH += "$$PWD/LabSFML/include"
    LIBS += -L"$$PWD/LabSFML/lib"
}

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

SOURCES += main.cpp \
    window.cpp \
    util.cpp \
    differentialevolver.cpp \
    binaryselector.cpp

HEADERS += \
    window.h \
    util.h \
    differentialevolver.h \
    binaryselector.h

QMAKE_CXXFLAGS += -O3
