QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuiExample
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \

HEADERS  += dialog.h \

LIBS += -L$$PWD/../../lib/ -lQtMailer

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

PRE_TARGETDEPS += $$PWD/../../lib/libQtMailer.a
