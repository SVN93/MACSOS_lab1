#-------------------------------------------------
#
# Project created by QtCreator 2015-03-26T22:07:13
#
#-------------------------------------------------

QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = macsos_lab1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
#    wavfile.cpp \
    utils.cpp \
    wavfile.cpp

HEADERS  += mainwindow.h \
#    wavfile.h \
    utils.h \
    wavfile.h

FORMS    += mainwindow.ui
