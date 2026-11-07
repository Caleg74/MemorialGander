TEMPLATE = app
TARGET = tst_ranking
QT += core testlib qml quick
CONFIG += testcase console c++17
CONFIG -= app_bundle
INCLUDEPATH += ../src/gymnast ../src/cbbItems ../src
SOURCES += tst_ranking.cpp ../src/gymnast/athletedata.cpp ../src/gymnast/singlemwdata.cpp ../src/arithmetictools.cpp
HEADERS += ../src/gymnast/athletedata.h ../src/gymnast/singlemwdata.h ../src/gymnast/rankingrules.h
