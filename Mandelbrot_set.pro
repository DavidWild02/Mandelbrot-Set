#-------------------------------------------------
#
# Project created by QtCreator 2018-10-30T13:42:28
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QMAKE_CXXFLAGS += -std=c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mandelbrot_Set
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mandelbrot.cpp \
    renderthread.cpp \
    zeichenflaeche.cpp \
    coloring.cpp


HEADERS  += mainwindow.h \
    mandelbrot.h \
    zeichenflaeche.h \
    renderthread.h \
    coloring.h

FORMS    += mainwindow.ui \
    mandelbrot.ui \
    zeichenflaeche.ui
