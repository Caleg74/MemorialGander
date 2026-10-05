import QtQuick
import QtQuick.Controls

Item {
    width: parent.width
    height: parent.height


    Rectangle {
        height: parent.height
        width: parent.width
        id: root
        color: "#d0d0d0"
        anchors.fill: parent
//        opacity: 1 - backendHelper.opacity

        Grid
        {
            columns: 2
            anchors.verticalCenter: parent.verticalCenter
            spacing: 20

            // Row 1
            Text {
                id: idCreateLastApparatusPdf
                width: 150
                text: qsTr("Attrezzo finale")
                font.pointSize: 15
            }

            StyleMGPushButton {
                id:btnCreateLastApparatusPdf
                objectName: "btnCreateLastApparatusPdf"
                width: 100
                buttonText: "Crea PDF"
            }

            // Row 2
            Text {
                id: idCreatePdf
                width: 150
                text: qsTr("Classifiche")
                font.pointSize: 15
            }

            StyleMGPushButton {
                id:btnCreatePdf
                objectName: "btnCreatePdf"
                width: 100
                buttonText: "Crea PDF"
            }
        }
    }
}
