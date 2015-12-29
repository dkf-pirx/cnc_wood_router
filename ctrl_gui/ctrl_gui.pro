#-------------------------------------------------
#
# Project created by lniestor
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = CncCtrl
TEMPLATE = app


SOURCES += sources/main.cpp\
           sources/mainwindow.cpp \
           sources/drivercomm.cpp \
           sources/settingsdialog.cpp

HEADERS  += sources/mainwindow.h \
            sources/drivercomm.h \
            sources/settingsdialog.h

FORMS    += ui/mainwindow.ui \
        ui/settingsdialog.ui
