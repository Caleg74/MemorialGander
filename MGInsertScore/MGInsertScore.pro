TEMPLATE = app
TARGET = MGInsertScore
include(backend.pri)
SOURCES += src/main.cpp
RC_ICONS = ../MGCommon/image/ApplicationGreen.ico

QML_IMPORT_PATH += src/qml
include(deployment.pri)

OTHER_FILES += \
    ../MGCommon/qml-styles/StyleMGComboBox.qml \
    ../MGCommon/qml-styles/StyleMGCheckbox.qml \
    ../MGCommon/qml-styles/StyleMGTextInput.qml \
    ../MGCommon/qml-styles/StyleMGPushButton.qml \
    ../MGCommon/qml-styles/StyleMGMsgBox.qml \
    src/qml/InsertScore.qml \
    src/qml/main.qml
