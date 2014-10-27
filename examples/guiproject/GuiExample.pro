QT       += core gui network

CONFIG += c++11

DEFINES += DEBUG


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuiExample
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    ../../src/QtMailer/mailer.cpp \
    ../../src/QtMailer/mail.cpp \
    ../../src/QtMailerStatus/mailerstatus.cpp

HEADERS  += dialog.h \
    ../../src/QtMailer/mailer.h \
    ../../src/QtMailer/mail.h \
    ../../src/QtMailerStatus/mailerstatus.h \
    ../../src/QtMailerStatus/mailerstatusStrings.h
