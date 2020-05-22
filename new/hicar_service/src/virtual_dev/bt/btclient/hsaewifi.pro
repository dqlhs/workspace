#-------------------------------------------------
#
# Project created by QtCreator 2019-11-19T12:12:21
#
#-------------------------------------------------

QT       -= core gui

TARGET = hsaewifi
TEMPLATE = lib

DEFINES += HSAEWIFI_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += hsaewifi.cpp \
    sppClient.c \
    bleClient.c \
    src/hsaewifi.cpp \
    src/sppClient.c \
    src/spp.c \
    src/hsaeble.c \
    src/device.c

HEADERS +=\
        hsaewifi_global.h \
    sppClient.h \
    include/sppClient.h \
    include/hsaewifi.h \
    include/bleClient.h \
    include/spp.h \
    include/hsaeble.h \
    include/driver_adapt.h \
    include/device.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
