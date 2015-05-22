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
    utils.cpp \
    wavfile.cpp

HEADERS  += mainwindow.h \
    utils.h \
    wavfile.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/qwt-6.1.2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.2/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.2/lib/debug/ -lqwt
else:mac: LIBS += -F$$PWD/../../../../usr/local/qwt-6.1.2/lib/ -framework qwt
else:unix: LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.2/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../usr/local/qwt-6.1.2
DEPENDPATH += $$PWD/../../../../usr/local/qwt-6.1.2
