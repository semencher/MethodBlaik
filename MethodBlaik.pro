QT += core
QT -= gui

CONFIG += c++11

TARGET = MethodBlaik
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    BoolClasses/BoolInterval/boolinterval.cpp \
    functionofmethod.cpp

HEADERS += \
    BoolClasses/BoolInterval/boolinterval.h \
    global.h \
    functionofmethod.h

SUBDIRS += \
    BoolClasses \
    BoolClasses/BoolInterval
