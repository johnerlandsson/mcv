#-------------------------------------------------
#
# Project created by QtCreator 2015-10-12T14:55:01
#
#-------------------------------------------------

QT       += opengl


CONFIG += c++11

TARGET = qmcv
TEMPLATE = lib

DEFINES += LIBQMCV_LIBRARY

SOURCES += libqmcv.cpp \
    matglwidget.cpp

HEADERS += libqmcv.h\
        libqmcv_global.h \
    matglwidget.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -L../lib -lmcv
INCLUDEPATH += ../include
