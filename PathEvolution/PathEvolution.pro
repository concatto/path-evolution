TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

SOURCES += main.cpp \
    window.cpp \
    util.cpp \
    differentialevolver.cpp

HEADERS += \
    window.h \
    util.h \
    differentialevolver.h

QMAKE_CXXFLAGS += -O3
