TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17 -Wall -Wfatal-errors

HEADERS += \
    headers/earth.h \
    headers/geometry.h \
    headers/logs.h \
    headers/points.h \
    headers/position.h \
    headers/route.h \
    headers/track.h \
    headers/types.h \
    headers/parseGPX.h \
    headers/gridworld/gridworld_model.h \
    headers/gridworld/gridworld_route.h \
    headers/gridworld/gridworld_track.h \
    headers/xml/element.h \
    headers/xml/generator.h \
    headers/xml/parser.h

SOURCES += \
    src/earth.cpp \
    src/geometry.cpp \
    src/logs.cpp \
    src/position.cpp \
    src/route.cpp \
    src/track.cpp \
    src/parseGPX.cpp \
    src/gridworld/gridworld_model.cpp \
    src/gridworld/gridworld_route.cpp \
    src/gridworld/gridworld_track.cpp \
    src/xml/element.cpp \
    src/xml/generator.cpp \
    src/xml/parser.cpp \
    tests/route/totalLength.cpp

SOURCES += \
    tests/route/route-tests.cpp \
    tests/route/numpoints.cpp \
    tests/route/indexing.cpp

INCLUDEPATH += headers/ headers/xml/ headers/gridworld

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = route-tests

LIBS += -lboost_unit_test_framework
