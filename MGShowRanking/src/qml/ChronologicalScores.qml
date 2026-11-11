pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls

Item {
//    width: parent.width
//    height: parent.height
    id: idChronoScores
    objectName: "idChronoScores"
    readonly property real columnWidth: Math.max(0, width - 10) / (12 + 2 / 3)
    readonly property int columnFontSize: Math.max(width / 120, 6)

    Rectangle {
        id: chronoScoresTitleleSpace
        y: 50
        height:  parent.height / 10
        width: parent.width
        radius: 5
        border.width: 2
        border.color: "#0a3f60"
        color: "#fff"

        Item // some sort of a Row
        {
            readonly property real colWidthUnit: idChronoScores.columnWidth
            anchors.margins: 5
            anchors.fill: parent
            readonly property int fontSize: idChronoScores.columnFontSize

/*
            Sesso | Nome Cognome | Nazione | Attrezzo | Coeff. difficoltà | Esecuzione | Bonus | Penalità | Nota finale | Totale
 */
            TableTitleItem {
                id: mwId
                text: "M/F"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit * 2 / 3
//                imageSource: "/images/Empty.svg" // no image, but reserve space
                anchors.left: parent.left
            }

            TableTitleItem {
                id: fullnameId
                objectName: "fullnameId"
                text: "Nome"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit * 3
//                imageSource: "/images/Empty.svg"
                anchors.left: mwId.right
            }

            TableTitleItem {
                id: countryId
                objectName: "countryId"
                text: "Nazione"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit
//                imageSource: "/images/Empty.svg"
                anchors.left: fullnameId.right
            }

            TableTitleItem {
                id: apparatusId
                objectName: "apparatusId"
                text: "Attrezzo"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit * 2
//                imageSource: "/images/Empty.svg"
                anchors.left: countryId.right
            }

            TableTitleItem {
                id: difficultyScoreId
                objectName: "difficultyScoreId"
                text: "Coeff. \ndiff."
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit
//                imageSource: "/images/Empty.svg"
                anchors.left: apparatusId.right
            }

            TableTitleItem {
                id: exeScoreId
                objectName: "ExeScoreId"
                text: "Esecuz."
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit
//                imageSource: "/images/Empty.svg"
                anchors.left: difficultyScoreId.right
            }

            TableTitleItem {
                id: bonusScoreId
                objectName: "BonusScoreId"
                text: qsTr("Bonus")
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit
                anchors.left: exeScoreId.right
            }

            TableTitleItem {
                id: penaltyScoreId
                objectName: "PenaltyScoreId"
                text: "Penalità"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit
//                imageSource: "/images/Empty.svg"
                anchors.left: bonusScoreId.right
            }
            TableTitleItem {
                id: finalScoreId
                objectName: "finalScoreId"
                text: "Nota \nfinale"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit
//                imageSource: "/images/Empty.svg"
                anchors.left: penaltyScoreId.right
            }

            TableTitleItem {
                id: totalGymnastId
                objectName: "totalGymnastId"
                text: "Totale"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.colWidthUnit
//                imageSource: "/images/Empty.svg"
                anchors.left: finalScoreId.right
                anchors.right: parent.right
                separatorVisible: false
            }

//            TableTitleItem {
//                id: rankId
//                objectName: "rankId"
//                text: "Rango"
//                fontSize: parent.fontSize
//                height: parent.height
//                width: parent.colWidthUnit
////                imageSource: "/images/Empty.svg"
//                anchors.left: totalGymnastId.right
//                separatorVisible: false
//            }
        }
    }

    ScrollView {
        width: parent.width
        height: parent.height
        anchors.top: chronoScoresTitleleSpace.bottom
        anchors.bottom: parent.bottom

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        ListView {
    //            width: 200; height: 250
            id: chronoview
            model: ChronoListModel
            delegate: ChronoListDelegate {
                columnWidth: idChronoScores.columnWidth
                columnFontSize: idChronoScores.columnFontSize
            }
    //                anchors.fill: parent
            width: parent.width
            spacing: 2

            // Animations
    //                add: Transition { NumberAnimation { properties: "y"; from:-50 ; duration: 500 } }
    //                removeDisplaced: Transition { NumberAnimation { properties: "y"; duration: 150 } }
    //                remove: Transition { NumberAnimation { property: "opacity"; to: 0; duration: 200 } }
        }

//        style: ScrollViewStyle {
//            transientScrollBars: true
//            handle: Item {
//                implicitWidth: 14
//                implicitHeight: 26
//                Rectangle {
//                    color: "grey"
//                    anchors.fill: parent
//                    anchors.topMargin: 6
//                    anchors.leftMargin: 4
//                    anchors.rightMargin: 4
//                    anchors.bottomMargin: 6
//                }
//            }
//            scrollBarBackground: Item {
//                implicitWidth: 14
//                implicitHeight: 26
//            }
//        }
    }
}

