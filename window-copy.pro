#-------------------------------------------------
#
# Project created by QtCreator 2013-11-06T20:35:47
#
#-------------------------------------------------
CONFIG   += qxt
QXT      += core gui
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = window-copy
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    preferences.cpp \
    windowcontentcopy.cpp \
    windowutils.cpp

HEADERS  += dialog.h \
    preferences.h \
    windowcontentcopy.h \
    windowutils.h

FORMS    += dialog.ui \
    preferences.ui \
    windowcontentcopy.ui

mac:LIBS += -framework ApplicationServices

#INCLUDEPATH += /usr/include/qxt
#LIBS +
