import QtQuick

Item {
    id: chronoDelegate
    required property real columnWidth
    required property int columnFontSize
    required property var model

    width: ListView.view?.width ?? 0
    height: 45


    Rectangle {
        y: 20
        height:  parent.height
        width: parent.width
        radius: 5
        border.width: 2
        border.color: "#0a3f60"
        color: "#fff"

        Gradient { // enabled or disabled depending on "latestScore"Changed" value
              id: grad
              GradientStop { position: 0.0; color: "#4083d1f5" }
              GradientStop { position: 1.0; color: "#A083d1f5" }
        }

        Rectangle // some sort of a Row
        {
            id: chronoRow
            readonly property real colWidthUnit: chronoDelegate.columnWidth
            anchors.margins: 5
            anchors.fill: parent
//            height: parent.height
//            width: parent.width
            readonly property int fontSize: chronoDelegate.columnFontSize
            gradient: chronoDelegate.model.LatestScore ? grad : null

            GeneralTxtItem {
                id: mwId
                generalText: chronoDelegate.model.MenWomen
                fontSize: parent.fontSize
                width: parent.colWidthUnit * 2 / 3
                anchors.left: parent.left
            }

            GymnastPosItem {
                id: nameId
                nameFull: chronoDelegate.model.NameFull
                flagImage: chronoDelegate.model.FlagImage
                fontSize: parent.fontSize
                width: parent.colWidthUnit * 3
                anchors.left: mwId.right
            }

            GeneralTxtItem {
                id: countryId
                generalText: chronoDelegate.model.Country
                fontSize: parent.fontSize
                width: parent.colWidthUnit
                anchors.left: nameId.right
            }

            GeneralTxtItem {
                id: apparatusId
                generalText: chronoDelegate.model.Apparatus
                fontSize: parent.fontSize
                width: parent.colWidthUnit * 2
                anchors.left: countryId.right
            }

            GeneralTxtItem {
                id: difficultyScoreId
                generalText: chronoDelegate.model.DifficultyScore
                fontSize: parent.fontSize
                fontBold: true
                width: parent.colWidthUnit
                anchors.left: apparatusId.right
            }

            GeneralTxtItem {
                id: exeScoreId
                generalText: chronoDelegate.model.ExecutionScore
                fontSize: parent.fontSize
                width: parent.colWidthUnit
                anchors.left: difficultyScoreId.right
            }

            GeneralTxtItem {
                id: bonusScoreId
                generalText: chronoDelegate.model.BonusScore
                fontSize: parent.fontSize
                width: parent.colWidthUnit
                anchors.left: exeScoreId.right
            }

            GeneralTxtItem {
                id: penaltyScoreId
                generalText: chronoDelegate.model.PenaltyScore
                fontSize: parent.fontSize
                width: parent.colWidthUnit
                anchors.left: bonusScoreId.right
            }

            GeneralTxtItem {
                id: finalScoreId
                generalText: chronoDelegate.model.FinalScore
                fontSize: parent.fontSize
                fontBold: true
                width: parent.colWidthUnit
                anchors.left: penaltyScoreId.right
            }

            GeneralTxtItem {
                id: gymnastTotId
                generalText: chronoDelegate.model.GymnastTotalScore
                fontSize: parent.fontSize
                width: parent.colWidthUnit
                anchors.left: finalScoreId.right
                anchors.right: parent.right
                separatorVisible: false
            }

//            GeneralTxtItem {
//                id: rankId
//                generalText: Rank   // roleName
//                fontSize: parent.fontSize
//                width: parent.colWidthUnit
//                anchors.left: gymnastTotId.right
//                anchors.right: parent.right
//            }
        }
    }
}
