import QtQuick
import QtQuick.Controls

Item {
    width: parent.width
    height: parent.height


    Rectangle {
        height: parent.height
        width: parent.width
        id: root
        color: "#d0d0d0"
        anchors.fill: parent
//        opacity: 1 - backendHelper.opacity


        Rectangle {
            id: addGymnastSpace
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
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter

                StyleMGTextInput {
                    id: txtFirstName
                    width: parent.parent.width/4 - 50
                    activeFocusOnPress: true
                    placeholderText: "Nome.."
                }

                StyleMGTextInput {
                    id: txtLastName
                    width: parent.parent.width/4 - 50
                    activeFocusOnPress: true
                    placeholderText: "Cognome.."
                }

                StyleMGComboBox {
                    id: cbbCountry
                    model: countryListModel.comboList
                    width: parent.parent.width/4 - 50
                    currentIndex: 0

//                    model: ListModel {
//                        id: cbbCountryItem
//                        ListElement { text: "Nazione.."}
//                        ListElement { text: "Svizzera" }
//                        ListElement { text: "Francia" }
//                        ListElement { text: "Italia" }
//                    }
                }

                StyleMGComboBox {
                    id: cbbSex
                    width: parent.parent.width/4 - 50
                    currentIndex: 0

                    model: ListModel {
                        id: cbbSexItem
                        ListElement { text: "Sesso.."}
                        ListElement { text: "M" }
                        ListElement { text: "F" }
                    }
                }

                Item {
                    id: btnAddGymnast
                    objectName: "btnAddGymnast"

                    width: 40 ; height: 40
                    enabled: (txtFirstName.text.length) && (txtLastName.text.length)
                            && (cbbCountry.currentIndex > 0) && (cbbSex.currentIndex > 0)
                    Image {
                        source: addMouseArea.pressed ? "qrc:images/add_icon_pressed.png" : "qrc:images/add_icon.png"
                        anchors.centerIn: parent
                        opacity: enabled ? 1 : 0.5
                    }
                    MouseArea {
                        id: addMouseArea
                        anchors.fill: parent
                        onClicked: {
                            const added = GymnastDataModel.sourceModel.addItem(
                                            txtFirstName.text, txtLastName.text,
                                            cbbCountry.currentText, cbbSex.currentText)
                            if (added) {
                                // Remove fields only after a successful insert.
                                txtFirstName.text = ""
                                txtLastName.text = ""
                                cbbCountry.currentIndex = 0
                                cbbSex.currentIndex = 0
                            }
                        }
                    }
                }
            }
        }

        ScrollView {
            width: parent.width
            height: parent.height
            anchors.top: addGymnastSpace.bottom
            anchors.bottom: root.bottom

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn


            ListView {
    //            width: 200; height: 250
                id: listviewinput
                model: GymnastDataModel
                delegate: GymnastInfoDelegate { }
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

