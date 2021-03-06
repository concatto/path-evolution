TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32 {
    INCLUDEPATH += "$$PWD/LabSFML/include"
    LIBS += -L"$$PWD/LabSFML/lib"
}

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lpthread

SOURCES += main.cpp \
    window.cpp \
    util.cpp \
    differentialevolver.cpp \
    binaryselector.cpp \
    numericinput.cpp \
    weightedbinaryselector.cpp \
    button.cpp

HEADERS += \
    window.h \
    util.h \
    differentialevolver.h \
    binaryselector.h \
    numericinput.h \
    weightedbinaryselector.h \
    button.h

QMAKE_CXXFLAGS += -O3 -pthread
