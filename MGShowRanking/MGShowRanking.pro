TEMPLATE = app

QT += sql qml quick quickcontrols2 widgets

CONFIG += c++11

RC_ICONS = ../MGCommon/image/Application.ico

SOURCES += \
    ../MGCommon/src/cbbItems/apparatusdata.cpp \
    ../MGCommon/src/cbbItems/apparatuslist.cpp \
    ../MGCommon/src/db/dbconnection.cpp \
    ../MGCommon/src/db/dbifacebase.cpp \
    ../MGCommon/src/cbbItems/comboboxmodel.cpp \
    ../MGCommon/src/cbbItems/countrylist.cpp \
    ../MGCommon/src/cbbItems/gymnasteventlist.cpp \
    ../MGCommon/src/gymnast/athletedata.cpp \
    ../MGCommon/src/gymnast/singlemwdata.cpp \
    ../MGCommon/src/gymnast/chronolistdata.cpp \
    ../MGCommon/src/arithmetictools.cpp \
    src/gymnastdatamodel.cpp \
    src/allroundmendatamodel.cpp \
    src/sortfilterproxymodel.cpp \
    src/allroundwomendatamodel.cpp \
    src/singlemwdatamodel.cpp \
    src/coreapplication.cpp \
    src/dbinterface.cpp \
    src/chronolistdatamodel.cpp \
    src/main.cpp

HEADERS += \
    ../MGCommon/src/cbbItems/apparatusdata.h \
    ../MGCommon/src/cbbItems/apparatuslist.h \
    ../MGCommon/src/db/dbconnection.h \
    ../MGCommon/src/db/dbifacebase.h \
    ../MGCommon/src/cbbItems/comboboxmodel.h \
    ../MGCommon/src/cbbItems/countrylist.h \
    ../MGCommon/src/cbbItems/gymnasteventlist.h \
    ../MGCommon/src/gymnast/athletedata.h \
    ../MGCommon/src/gymnast/singlemwdata.h \
    ../MGCommon/src/gymnast/chronolistdata.h \
    ../MGCommon/src/arithmetictools.h \
    src/gymnastdatamodel.h \
    src/allroundmendatamodel.h \
    src/sortfilterproxymodel.h \
    src/allroundwomendatamodel.h \
    src/singlemwdatamodel.h \
    src/coreapplication.h \
    src/dbinterface.h \
    src/chronolistdatamodel.h


INCLUDEPATH += \
    ../MGCommon/src \
    ../MGCommon/src/db \
    ../MGCommon/src/cbbItems \
    ../MGCommon/src/gymnast \
    src/

RESOURCES += MGShowRanking.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH = src/qml

QT_QUICK_CONTROLS_STYLE=Flat ./app

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    ../MGCommon/qml-styles/StyleMGTabWidget.qml \
    ../MGCommon/qml-styles/StyleMGComboBox.qml \
    ../MGCommon/qml-styles/StyleMGTextInput.qml \
    ../MGCommon/qml-styles/StyleMGPushButton.qml \
    src/qml/AllroundMDelegate.qml \
    src/qml/AllroundWDelegate.qml \
    src/qml/ChronoListDelegate.qml \
    src/qml/SingleMWDelegate.qml \
    src/qml/MenRanking.qml \
    src/qml/WomenRanking.qml \
    src/qml/ApparatusRanking.qml \
    src/qml/ApparatusRankingM.qml \
    src/qml/ApparatusRankingW.qml \
    src/qml/ChronologicalScores.qml \
    src/qml/TableTitleItem.qml \
    src/qml/ScoreItem.qml \
    src/qml/GymnastPosItem.qml \
    src/qml/GeneralTxtItem.qml \
    src/qml/main.qml
