import QtQuick
import QtQuick.Controls

Item {
//    width: parent.width
//    height: parent.height


    Item {  // some sort of a Row

        id: selbarW
        height: 100
        width: parent.width
        anchors.margins: 10
        anchors.left: parent.left
        anchors.right: parent.right


        Text{
            id: lblTitleWomen
            renderType: Text.NativeRendering
            text: "Attrezzo Femminile: "
            font.pixelSize: Math.max(parent.width / 20, 6)
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
        }

        StyleMGComboBox {
            id: cbbAppartusW
            objectName: "cbbAppartusW"
            model: apparatusModelW.comboList
            width: parent.parent.width /2
            currentIndex: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: lblTitleWomen.right
            signal selectedTextChanged(string currentTxt)
            onCurrentTextChanged: {
                selectedTextChanged(currentText)
            }
            onEnabledChanged: {
                if (enabled === true)
                    selectedTextChanged(currentText)
            }
        }
    }

    Rectangle {
        id: singleWomenTitleleSpace
        y: 50
        height:  parent.height / 12
        width: parent.width
        radius: 5
        border.width: 2
        border.color: "#0a3f60"
        color: "#fff"
        anchors.top: selbarW.bottom

        Item // some sort of a Row
        {
            readonly property real apparatusWidth: parent.width * 0.46
            anchors.margins: 5
            anchors.fill: parent
    //                height: parent.height
    //                width: parent.width
            property int fontSize:  Math.max(parent.width / 33, 6)


            TableTitleItem {
                id: gymnWId
                text: "Ginnasta"
                fontSize: parent.fontSize
                height: parent.height
                imageSource: "/images/Empty.svg" // no image, but reserve space
                anchors.left: parent.left
                anchors.right: singleApparatusWId.left
            }

            TableTitleItem {
                id: singleApparatusWId
                objectName: "singleApparatusWId"
                text: "Attrezzo"
                fontSize: parent.fontSize
                height: parent.height
                width: parent.apparatusWidth
                imageSource: "/images/Empty.svg"
                anchors.right: parent.right
                separatorVisible: false
            }
        }
    }

    ScrollView {
        width: parent.width
        height: parent.height
        anchors.top: singleWomenTitleleSpace.bottom
        anchors.bottom: parent.bottom

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff


        ListView {
    //            width: 200; height: 250
            id: rankingview
            model: SingleWDataModel
            delegate: SingleMWDelegate { }
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

