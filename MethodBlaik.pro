QT += core
QT -= gui

CONFIG += c++11

TARGET = MethodBlaik
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    BoolClasses/BoolVector/boolvector.cpp \
    BoolClasses/BoolInterval/boolinterval.cpp \
    functionofmethod.cpp

HEADERS += \
    BoolClasses/BoolVector/boolvector.h \
    BoolClasses/BoolInterval/boolinterval.h \
    global.h \
    functionofmethod.h

SUBDIRS += \
    BoolClasses \
    BoolClasses/BoolVector \
    BoolClasses/BoolInterval
