import QtQuick
import QtQuick.Controls.Basic

TextField {
    id: control
    property alias backColor: idBackColor.color

    placeholderText: qsTr("Enter description")
    color: "#0a3f60"
    placeholderTextColor: "#83d1f5"
    font.pointSize: 15

    background: Rectangle {
        id: idBackColor
        color: control.readOnly ? "transparent" : "white"
        border.width: 2
        border.color: "#0a3f60"
        radius: 5
    }
}
