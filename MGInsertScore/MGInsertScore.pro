TEMPLATE = app

QT += sql qml quick widgets

CONFIG += c++11

RC_ICONS = ../MGCommon/image/ApplicationGreen.ico

HEADERS += \
    ../MGCommon/src/cbbItems/apparatusdata.h \
    ../MGCommon/src/cbbItems/apparatuslist.h \
    ../MGCommon/src/cbbItems/comboboxmodel.h \
    ../MGCommon/src/cbbItems/gymnasteventlist.h \
    ../MGCommon/src/db/dbconnection.h \
    ../MGCommon/src/db/dbifacebase.h \
    ../MGCommon/src/msgBox/messagebox.h \
    src/coreapplication.h \
    src/dbinterface.h \
    src/savescore.h

SOURCES += \
    ../MGCommon/src/cbbItems/apparatusdata.cpp \
    ../MGCommon/src/cbbItems/apparatuslist.cpp \
    ../MGCommon/src/db/dbconnection.cpp \
    ../MGCommon/src/db/dbifacebase.cpp \
    ../MGCommon/src/cbbItems/comboboxmodel.cpp \
    ../MGCommon/src/cbbItems/gymnasteventlist.cpp \
    ../MGCommon/src/msgBox/messagebox.cpp \
    src/coreapplication.cpp \
    src/dbinterface.cpp \
    src/main.cpp \
    src/savescore.cpp


INCLUDEPATH += \
    ../MGCommon/src/db \
    ../MGCommon/src/cbbItems \
    ../MGCommon/src/msgBox \
    src/

RESOURCES += MGInsertScore.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = src/qml

QT_QUICK_CONTROLS_STYLE=Flat ./app

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    ../MGCommon/qml-styles/StyleMGTabWidget.qml \
    ../MGCommon/qml-styles/StyleMGCheckbox.qml \
    ../MGCommon/qml-styles/StyleMGComboBox.qml \
    ../MGCommon/qml-styles/StyleMGTextInput.qml \
    ../MGCommon/qml-styles/StyleMGPushButton.qml \
    ../MGCommon/qml-styles/StyleMGMsgBox.qml \
    src/qml/InsertScore.qml \
    src/qml/main.qml
