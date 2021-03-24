TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17 -Wall -Wfatal-errors

HEADERS += \
    headers/earth.h \
    headers/geometry.h \
    headers/logs.h \
    headers/parseNMEA.h \
    headers/position.h \
    headers/types.h

SOURCES += \
    src/earth.cpp \
    src/geometry.cpp \
    src/logs.cpp \
    src/parseNMEA.cpp \
    src/position.cpp \
    
SOURCES += \
    tests/parseNMEA-tests.cpp

INCLUDEPATH += headers/

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = parseNMEA-tests

LIBS += -lboost_unit_test_framework
