QT          +=  core \
                gui \
                network \
                widgets

TEMPLATE    =   lib
TARGET      =   QtMailer

DESTDIR     =   ../lib

CONFIG      +=  qt
CONFIG      +=  staticlib
CONFIG      +=  c++11

HEADERS     =   mail.h \
                mailer.h \
                mailerstatus.h \
                mailerstatusStrings.h

SOURCES     =   mail.cpp \
                mailer.cpp \
                mailerstatus.cpp

unix {
    isEmpty(PREFIX){
        PREFIX = /usr
    }
    INCLUDEPATH     =   $$PREFIX/include
    target.path     =   $$PREFIX/lib
    INSTALLS        +=  target

    headers.files   =   $$HEADERS
    headers.path    =   $$PREFIX/include/$$TARGET
    INSTALLS        +=  headers
}
