TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17 -Wall -Wfatal-errors

HEADERS += \
    headers/earth.h \
    headers/geometry.h \
    headers/logs.h \
    headers/parseGPX.h \
    headers/points.h \
    headers/position.h \
    headers/route.h \
    headers/track.h \
    headers/types.h \
    headers/xml/element.h \
    headers/xml/parser.h

SOURCES += \
    apps/consoleApp.cpp

SOURCES += \
    src/earth.cpp \
    src/geometry.cpp \
    src/logs.cpp \
    src/parseGPX.cpp \
    src/position.cpp \
    src/route.cpp \
    src/track.cpp \
    src/xml/element.cpp \
    src/xml/parser.cpp


INCLUDEPATH += headers/ headers/xml/

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = console-app
