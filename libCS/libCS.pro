#-------------------------------------------------
#
# Project created by QtCreator 2017-11-23T16:26:17
#
#-------------------------------------------------

QT       += network sql xml widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = CS
TEMPLATE = lib

DEFINES += LIBCS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        server.cpp \
        threadserver.cpp \
        user.cpp \
        channel.cpp \
        threadclient.cpp \
        client.cpp \
    message.cpp

HEADERS += \
        libcs.h \
        server.h \
        threadserver.h \
        user.h \
        channel.h \
        threadclient.h \
        client.h \
        libcs_global.h \   
    message.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
