#-------------------------------------------------
#
# Project created by QtCreator 2015-10-12T15:03:48
#
#-------------------------------------------------

QT       += core gui opengl sql

CONFIG += c++11 console

#DEFINES += USE_IO

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCornerGuardInspector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageprocessor.cpp \
    barcodesettings.cpp \
    barcodesettingsdialog.cpp \
    camerasettings.cpp \
    camerasettingsdialog.cpp \
    generalsettings.cpp \
    generalsettingsdialog.cpp \
    holesettings.cpp \
    holesettingsdialog.cpp \
    alarmstablemodel.cpp \
    point_operations.cpp \
    io.cpp \
    PCI7260.cpp

HEADERS  += mainwindow.h \
    imageprocessor.h \
    barcodesettings.h \
    barcodesettingsdialog.h \
    camerasettings.h \
    camerasettingsdialog.h \
    generalsettings.h \
    generalsettingsdialog.h \
    holesettings.h \
    holesettingsdialog.h \
    alarmstablemodel.h \
    point_operations.h \
    io.h \
    PCI7260.h

FORMS    += mainwindow.ui \
    barcodesettingsdialog.ui \
    camerasettingsdialog.ui \
    generalsettingsdialog.ui \
    holesettingsdialog.ui

LIBS += -L../lib -lqmcv -lmcv -lNETUSBCAM -lopencv_core -lopencv_highgui -lopencv_imgproc -lzbar
INCLUDEPATH += ../include/

RESOURCES += \
    resource.qrc
