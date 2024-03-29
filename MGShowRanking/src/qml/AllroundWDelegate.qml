import QtQuick 2.0

Item {
    id: allroundMDelegate
    width: parent.width
    height: 45

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
            property int apparatusWidth: parent.width / 7
            anchors.margins: 5
            anchors.fill: parent
//            height: parent.height
//            width: parent.width
            property int fontSize:  Math.max(parent.width / 90, 6)


            GymnastPosItem {
                id: rankId
                rank: Rank           // roleName
                nameFull: NameFull   // roleName
                flagImage: FlagImage // roleName
                fontSize: parent.fontSize
                anchors.left: parent.left
                anchors.right: totId.left
            }

            ScoreItem {
                id: totId
                //difficultyScore: DifficultyScore_Total  // roleName
                finalScore: FinalScore_Total  // roleName
                isFinalApparatus: false
                latestScore: false
                fontSize: parent.fontSize
                fontBold: true
                width: parent.apparatusWidth
                anchors.right: vaultId.left
            }

            ScoreItem {
                id: vaultId
                difficultyScore: DifficultyScore_Vault  // roleName
                finalScore: FinalScore_Vault  // roleName
                isFinalApparatus: FinalApparatus_Vault // roleName
                latestScore: LatestScore_Vault         // roleName
                fontSize: parent.fontSize
                width: parent.apparatusWidth
                anchors.right: unevBarsId.left
            }

            ScoreItem {
                id: unevBarsId
                difficultyScore: DifficultyScore_UnBars  // roleName
                finalScore: FinalScore_UnBars  // roleName
                isFinalApparatus: FinalApparatus_UnBars // roleName
                latestScore: LatestScore_UnBars         // roleName
                fontSize: parent.fontSize
                width: parent.apparatusWidth
                anchors.right: balanceBId.left
            }

            ScoreItem {
                id: balanceBId
                difficultyScore: DifficultyScore_BalBeam  // roleName
                finalScore: FinalScore_BalBeam  // roleName
                isFinalApparatus: FinalApparatus_BalBeam // roleName
                latestScore: LatestScore_BalBeam         // roleName
                fontSize: parent.fontSize
                width: parent.apparatusWidth
                anchors.right: floorId.left
            }

            ScoreItem {
                id: floorId
                difficultyScore: DifficultyScore_Floor  // roleName
                finalScore: FinalScore_Floor  // roleName
                isFinalApparatus: FinalApparatus_Floor // roleName
                latestScore: LatestScore_Floor         // roleName
                fontSize: parent.fontSize
                width: parent.apparatusWidth
                anchors.right: parent.right
                separatorVisible: false
            }
        }
    }
}
