import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: parent.width
    height: parent.height

    Rectangle {
        height: 300
        width: 500
        id: root
        color: "#d0d0d0"
        anchors.leftMargin: 20

        Column
        {
            width: parent.width
            spacing: 40
            anchors.centerIn: parent

            Row
            {
                spacing: 20
//                anchors.verticalCenter: parent.verticalCenter


                StyleMGComboBox {
                    id: cbbGymnastSelection
                    objectName: "cbbGymnastSelection"
                    model: gymnastRegisteredModel.comboList
                    width: parent.parent.width/2
                    currentIndex: 0
                    signal selectedTextChanged(string currentTxt)
                    onCurrentTextChanged: {
                        selectedTextChanged(currentText)
                    }
                }

                StyleMGComboBox {
                    id: cbbAppartus
                    model: apparatusModel.comboList
                    width: parent.parent.width/2
                    visible: cbbGymnastSelection.currentIndex > 0
                    currentIndex: 0
                }
            }

            RowLayout
            {
                spacing: 40
                Layout.alignment: Qt.AlignBottom

                Text {
                    id: lblForceScore
//                    width: parent.parent.width/3
                    text: "In caso di parità al \n" +
                          "singolo attrezzo:"
                    font.pointSize: 15
                }

                StyleMGTextInput {
                    id: txtForceScore
                    width: 30
                    activeFocusOnPress: true
                    placeholderText: "0..X"
                }

                StyleMGPushButton {
                    id:btnSaveScore
                    objectName: "btnSaveScore"
                    width: 100
                    buttonText: "Registra"
                    signal saveScore(string gymnastName,
                                     string apparatus,
                                     string forceScore)

                    enabled: (txtForceScore.text.length)
                             && (cbbGymnastSelection.currentIndex >= 0)
                             && (cbbAppartus.currentIndex >= 0)
                    onClicked: {
                        saveScore(cbbGymnastSelection.currentText,
                                  cbbAppartus.currentText,
                                  txtForceScore.text)

                        txtForceScore.text = ""
                        cbbAppartus.currentIndex = 0


                    }
                }
            }

            Rectangle
            {
                width: parent.width
                height: 2 // just a line
                color: "#aaaaaa"
            }

            Row
            {
                Text {
                    id: lblEraseForceScore
//                    width: parent.parent.width/3
                    text: "Azzera tutti punteggi speciali:  "
                    font.pointSize: 15
//                    anchors.left: parent.left
                }

                StyleMGPushButton {
                    id:btnEraseForceScore
                    objectName: "btnEraseForceScore"
                    width: 100
                    buttonText: "Azzera"

//                    anchors.right: parent.right

                }
            }
        }
    }
}

