import QtQuick 2.0
import QtQuick.Controls 2.0
//import QtQuick.Particles 2.0

Item {
    id: root
    property string finalScore;
    property string startScore;
    property bool separatorVisible: true;
    property int fontSize: 15;

    width: parent.width
    height: parent.height

    Item {  // some sort of a Row
//        width: parent.width
//        height: parent.height
        anchors.fill: parent
        anchors.margins: 5

        Item {  // some sort of a Row
            width: parent.width
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
//            anchors.margins: 10
            anchors.left: parent.left
            anchors.right: separator.right

            Text {
                id: finalScore
                text: root.finalScore
                font.pointSize: fontSize
                color: "#0a3f60"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
//                width: parent.width - startScore.width - separator.width
                height: parent.height
                anchors.left: parent.left
                anchors.right: startScore.left
            }

            Text {
                id: startScore
                text: root.startScore // root.startScore !== "0" ? "("+ root.startScore +")" : ""
                font.pointSize: fontSize * 2 / 3
                color: "#0a3f60"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                width: parent.width * 1 /3
                height: parent.height
                anchors.right: parent.right

            }
        }
        Rectangle {
            id: separator
            width: 2    // just a line
            height: parent.height
            color: "#d0d0d0"
            visible: separatorVisible
            anchors.right: parent.right
        }
    }
}
