import QtQuick

Item {
    id: singleDelegate
    required property var model

    width: ListView.view?.width ?? 0
    height: 50

    Rectangle {
        y: 20
        height:  parent.height
        width: parent.width
        radius: 5
        border.width: 2
        border.color: "#0a3f60"
        color: "#fff"


        Item // some sort of a Row
        {
            readonly property real apparatusWidth: parent.width * 0.46
            anchors.margins: 5
            anchors.fill: parent
//            height: parent.height
//            width: parent.width
            property int fontSize:  Math.max(parent.width / 33, 6)


            GymnastPosItem {
                id: rankId
                rank: singleDelegate.model.Rank
                nameFull: singleDelegate.model.NameFull
                flagImage: singleDelegate.model.FlagImage
                fontSize: parent.fontSize
                anchors.left: parent.left
                anchors.right: singleScoreMId.left
            }


            ScoreItem {
                id: singleScoreMId
                scoreDetails: singleDelegate.model.ScoreDetails_SingleMW
                finalScore: singleDelegate.model.FinalScore_SingleMW
                fontSize: parent.fontSize
                width: parent.apparatusWidth
                separatorVisible: false
                anchors.right: parent.right
            }
        }
    }
}
