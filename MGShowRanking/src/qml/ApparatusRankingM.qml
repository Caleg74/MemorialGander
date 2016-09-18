import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.1

Item {
//    width: parent.width
//    height: parent.height


    Item {  // some sort of a Row

        id: selbarM
        height: 100
        width: parent.width
        anchors.margins: 10
        anchors.left: parent.left
        anchors.right: parent.right


        Text{
            id: lblTitleMen
            text: "Attrezzo Maschile: "
            font.pixelSize: Math.max(parent.width / 20, 6)
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
        }

        ComboBox {
            id: cbbAppartusM
            objectName: "cbbAppartusM"
            model: apparatusModelM.comboList
            width: parent.parent.width / 2
            activeFocusOnPress: true
            style:comboBoxMGStyle
            currentIndex: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: lblTitleMen.right
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
        id: singleMenTitleleSpace
        y: 50
        height:  parent.height / 12
        width: parent.width
        radius: 5
        border.width: 2
        border.color: "#0a3f60"
        color: "#fff"
        anchors.top: selbarM.bottom

        Item // some sort of a Row
        {
            property int apparatusWidth: parent.width / 2.5
            anchors.margins: 5
            anchors.fill: parent
    //                height: parent.height
    //                width: parent.width
            property int fontSize:  Math.max(parent.width / 33, 6)


            TableTitleItem {
                id: gymnMId
                text: "Ginnasta"
                fontSize: parent.fontSize
                height: parent.height
                imageSource: "/images/Empty.svg" // no image, but reserve space
                anchors.left: parent.left
                anchors.right: singleApparatusMId.left
            }

            TableTitleItem {
                id: singleApparatusMId
                objectName: "singleApparatusMId"
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
        anchors.top: singleMenTitleleSpace.bottom
        anchors.bottom: parent.bottom

        flickableItem.interactive: true

        ListView {
    //            width: 200; height: 250
            id: rankingview
            model: SingleMDataModel
            delegate: SingleMWDelegate { }
    //                anchors.fill: parent
            width: parent.width
            spacing: 2

            // Animations
    //                add: Transition { NumberAnimation { properties: "y"; from:-50 ; duration: 500 } }
    //                removeDisplaced: Transition { NumberAnimation { properties: "y"; duration: 150 } }
    //                remove: Transition { NumberAnimation { property: "opacity"; to: 0; duration: 200 } }
        }

        style: ScrollViewStyle {
            transientScrollBars: true
            handle: Item {
                implicitWidth: 14
                implicitHeight: 26
                Rectangle {
                    color: "grey"
                    anchors.fill: parent
                    anchors.topMargin: 6
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    anchors.bottomMargin: 6
                }
            }
            scrollBarBackground: Item {
                implicitWidth: 14
                implicitHeight: 26
            }
        }
    }

}
