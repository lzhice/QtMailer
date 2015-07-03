QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuiExample
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \

HEADERS  += dialog.h \

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lQtMailer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lQtMailer
else:unix: LIBS += -L$$PWD/../../lib/ -lQtMailer

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/libQtMailer.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/libQtMailer.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/QtMailer.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/QtMailer.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../lib/libQtMailer.a
