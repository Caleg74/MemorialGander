import QtQuick
import QtQuick.Controls

Item {
    width: parent.width
    height: parent.height


    Rectangle {
        height: parent.height
        width: parent.width
        id: rootGS
        color: "#d0d0d0"
        anchors.fill: parent
//        opacity: 1 - backendHelper.opacity


        Rectangle {
            id: selectGymnastSpace
            width: parent.width
            y: 50
            height: 70
            border.width: 5
            border.color: "#d0d0d0"
            radius: 1
            color: "#d0d0d0"

            Row
            {
                spacing: 10
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                StyleMGComboBox {
                    id: cbbGymnastSelection
                    model: gymnastRegisteredModel.comboList
                    width: parent.parent.width/2 - 50
                    currentIndex: 0
                }

                Item {
                    id: btnAddGymnastToEvent
                    objectName: "btnAddGymnasttoevent"

                    width: 40 ; height: 40
                    enabled: (cbbGymnastSelection.currentIndex > 0)
                    Image {
                        source: addMouseArea.pressed ? "qrc:images/add_icon_pressed.png" : "qrc:images/add_icon.png"
                        anchors.centerIn: parent
                        opacity: enabled ? 1 : 0.5
                    }
                    MouseArea {
                        id: addMouseArea
                        anchors.fill: parent
                        onClicked: {
                            const added = GymnastSelectModel.sourceModel.addItem(
                                            cbbGymnastSelection.currentText)
                            if (added)
                                cbbGymnastSelection.currentIndex = 0
                        }
                    }
                }
            }
        }

        ScrollView {
            width: parent.width
            height: parent.height
            anchors.top: selectGymnastSpace.bottom
            anchors.bottom: rootGS.bottom

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn


            ListView {
    //            width: 200; height: 250
                id: listviewGSinput
                model: GymnastSelectModel
                delegate: GymnastSelectionDelegate { }
                anchors.fill: parent
                width: parent.width
                clip: true


                // Animations
                add: Transition { NumberAnimation { properties: "y"; from:-50 ; duration: 500 } }
                removeDisplaced: Transition { NumberAnimation { properties: "y"; duration: 150 } }
                remove: Transition { NumberAnimation { property: "opacity"; to: 0; duration: 200 } }
            }

//            style: ScrollViewStyle {
//                transientScrollBars: true
//                handle: Item {
//                    implicitWidth: 14
//                    implicitHeight: 26
//                    Rectangle {
//                        color: "grey"
//                        anchors.fill: parent
//                        anchors.topMargin: 6
//                        anchors.leftMargin: 4
//                        anchors.rightMargin: 4
//                        anchors.bottomMargin: 6
//                    }
//                }
//                scrollBarBackground: Item {
//                    implicitWidth: 14
//                    implicitHeight: 26
//                }
//            }
        }
    }


}

