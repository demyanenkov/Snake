QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scene/scene.cpp \
    scene/snake.cpp

HEADERS += \
    mainwindow.h \
    scene/scene.h \
    scene/object.h \
    scene/view.h \
    scene/pos.h \
    scene/snake.h \
    scene/area.h \
    scene/bonus.h \
    scene/fruits.h

FORMS += \
    mainwindow.ui

#DISTFILES += \
#    Readme.txt

RESOURCES += \
    snake.qrc
