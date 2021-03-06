#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T19:48:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SubtitleManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    subtitleio.cpp \
    subtitles.cpp \
    srtio.cpp \
    interval.cpp \
    subtitle.cpp \
    tableviewmodel.cpp \
    dvdio.cpp \
    paramquestion.cpp \
    subio.cpp \
    subtitleprocessing.cpp \
    titlemerge.cpp \
    titlesplit.cpp \
    titleshift.cpp \
    infowindow.cpp

HEADERS  += mainwindow.h \
    subtitleio.h \
    subtitles.h \
    srtio.h \
    interval.h \
    subtitle.h \
    tableviewmodel.h \
    dvdio.h \
    paramquestion.h \
    subio.h \
    subtitleprocessing.h \
    titlemerge.h \
    titlesplit.h \
    titleshift.h \
    infowindow.h

FORMS    += mainwindow.ui \
    paramquestion.ui \
    titlemerge.ui \
    titlesplit.ui \
    titleshift.ui \
    infowindow.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
    icons/delete.ico \
    icons/edit.ico \
    icons/info.ico \
    icons/insert.ico \
    icons/new.ico \
    SubtitleManager.rc

RC_FILE = SubtitleManager.rc
