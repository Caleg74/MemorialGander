TEMPLATE = app
TARGET = tst_mginsertscore
include(../backend.pri)
QT += testlib
CONFIG += testcase console
CONFIG -= app_bundle qml_debug
SOURCES += $$PWD/tst_mginsertscore.cpp
