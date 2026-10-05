import QtQuick
import QtQuick.Controls.Basic

Dialog {
    id: messageDialog
    objectName: "messageDialog"

    property string text: "my text"

    modal: true
    standardButtons: Dialog.Ok
    title: "Warning"
    anchors.centerIn: parent

    width: 400
    height: 200

    Row {
        spacing: 30
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            source: "qrc:/images/warning_32x32.png"
        }

        Text {
            text: messageDialog.text
            font.pointSize: 12
            color: "#0a3f60"
        }
    }
}
