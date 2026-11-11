import QtQuick

Item {
    id: root
    property string finalScore;
    property string difficultyScore;
    property string scoreDetails
    readonly property bool hasDetails: scoreDetails.length > 0
    property bool separatorVisible: true;
    property int fontSize: 15;
    property bool fontBold: false;
    property bool isFinalApparatus: false;
    property bool latestScore

    width: parent.width
    height: parent.height

    onLatestScoreChanged: {
        if (latestScore) {
            scoreBackground.gradient = grad
        } else {
            scoreBackground.gradient = null
        }
   }

    Item {  // some sort of a Row
//        width: parent.width
//        height: parent.height
        anchors.fill: parent
        anchors.margins: 5
        anchors.topMargin: root.hasDetails ? 0 : 5
        anchors.bottomMargin: root.hasDetails ? 0 : 5

        Rectangle {
            id: scoreBackground
            width: parent.width
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
//            anchors.margins: 10
            anchors.left: parent.left
            anchors.right: separator.right
//            color: latestScore ? "#8083d1f5" : "white"
            radius: 5
            Gradient { // enabled or disabled depending on "latestScore"Changed" value
                  id: grad
                  GradientStop { position: 0.0; color: "#4083d1f5" }
                  GradientStop { position: 1.0; color: "#A083d1f5" }
            }

            Text {
                id: finalScore
                renderType: Text.NativeRendering
                text: root.finalScore
                font.pointSize: root.fontSize
                font.bold: root.fontBold
                font.italic: root.isFinalApparatus
                color:  "#0a3f60"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
//                width: parent.width - difficultyScore.width - separator.width
                height: parent.height
                anchors.left: parent.left
                anchors.right: difficultyScore.left
            }

            Text {
                id: difficultyScore
                renderType: Text.NativeRendering
                text: root.hasDetails ? root.scoreDetails : root.difficultyScore
                font.pointSize: root.hasDetails ? Math.max(root.fontSize, 11) : root.fontSize * 3 / 4
                fontSizeMode: root.hasDetails ? Text.Fit : Text.FixedSize
                minimumPointSize: 5
                font.bold: false
                font.italic: root.isFinalApparatus
                color:  "#0a3f60"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                width: root.hasDetails ? Math.max(0, parent.width - finalScore.implicitWidth - 6)
                                       : parent.width * 2 / 5
                height: parent.height
                anchors.right: parent.right
            }
        }
        Rectangle {
            id: separator
            width: 2    // just a line
            height: parent.height
            color: "#d0d0d0"
            visible: root.separatorVisible
            anchors.right: parent.right
        }
    }
}
