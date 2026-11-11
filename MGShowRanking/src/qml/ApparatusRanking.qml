import QtQuick

Item {
    id: idSingleRanking
    objectName: "idSingleRanking"
    readonly property real panelWidth: width * 0.45

    Rectangle {

        id: rootM
        color: "#d0d0d0"
        height: parent.height
        width: parent.width / 2
        anchors.left: parent.left

//        opacity: 1 - backendHelper.opacity

        ApparatusRankingM {  // some sort of a Column
            width: idSingleRanking.panelWidth
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    Rectangle {

        id: rootW
        color: "#d0d0d0"
        height: parent.height
        width: parent.width / 2
        anchors.right: parent.right

        ApparatusRankingW {  // some sort of a Column

            width: idSingleRanking.panelWidth
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

