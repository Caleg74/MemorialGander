import QtQuick
import QtQuick.Controls.Basic

Button {
    id: control
    implicitWidth: 30
    implicitHeight: 30
    property alias buttonText: control.text
    property color color: "white"
    property color hoverColor: "#aaaaaa"
    property color pressColor: "#83d1f5"
    property int fontSize: 15
    property int borderWidth: 2
    property int borderRadius: 5

    hoverEnabled: true
    scale: down ? 0.96 : 1.0
    Behavior on scale {
        NumberAnimation { duration: 100; easing.type: Easing.InOutQuad }
    }

    contentItem: Text {
        text: control.text
        font.pointSize: control.fontSize
        renderType: Text.NativeRendering
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "black"
    }

    background: Rectangle {
        radius: control.borderRadius
        color: !control.enabled ? "lightgrey"
               : control.down ? control.pressColor
               : control.hovered ? control.hoverColor : control.color
        border.width: control.borderWidth
        border.color: "#0a3f60"
        Behavior on color { ColorAnimation { duration: control.down ? 10 : 200 } }
    }
}
