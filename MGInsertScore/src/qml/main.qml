import QtQuick
import QtQuick.Window
import Memorial.InsertScore

Window {
    id: window
    required property ScoreController controller
    width: 800
    height: 500
    visible: true
    title: qsTr("Memorial Gander - Insert Score")


    Rectangle {
        property string title: "Registra \nGinnasti"
        anchors.fill: parent
        color: "#d0d0d0"

        InsertScore
        {
            controller: window.controller
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
        }
    }
}
