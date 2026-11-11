import QtQuick
import Memorial.Server

Item {
    id: eventData
    required property CoreApplication coreApplication

    Rectangle {
        color: "#d0d0d0"
        anchors.fill: parent

        Grid {
            columns: 2
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -25
            spacing: 20
            verticalItemAlignment: Grid.AlignVCenter

            Text {
                id: editionLabel
                width: 150
                text: qsTr("Edizione")
                font.pointSize: 15
            }

            StyleMGComboBox {
                id: editionCombo
                objectName: "editionComboBox"
                Accessible.name: editionLabel.text
                model: eventData.coreApplication.editions
                currentValue: eventData.coreApplication.currentEvent
                onActivated: {
                    if (!eventData.coreApplication.selectEvent(editionCombo.currentValue))
                        editionCombo.currentIndex = editionCombo.indexOfValue(eventData.coreApplication.currentEvent)
                }
            }

            Text {
                width: 150
                text: qsTr("Attrezzo finale")
                font.pointSize: 15
            }

            StyleMGPushButton {
                objectName: "btnCreateLastApparatusPdf"
                width: 100
                buttonText: "Crea PDF"
            }

            Text {
                width: 150
                text: qsTr("Classifiche")
                font.pointSize: 15
            }

            StyleMGPushButton {
                objectName: "btnCreatePdf"
                width: 100
                buttonText: "Crea PDF"
            }
        }
    }
}
