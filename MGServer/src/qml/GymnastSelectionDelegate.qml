pragma ComponentBehavior: Bound

import QtQuick

Item {
    id: listItemDelegate
    required property string fullName
    required property int athleteId
    required property int eventId
    required property var apparatusList
    required property int finalApparatusIndex

    width: (parent.width * 2/3) - 50
    height: 50

    BorderImage {
        id: item
        width: parent.width ; height: 50
        source: "qrc:images/delegate.png"
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5

        Row {
            id: delegateRow
            spacing: 20
            anchors.fill: parent

            Rectangle { // just to create a space in the row
                color: "transparent"
                height: 50; width: 50
            }

            Text {
                id: regFullNameText
                x: 50
                text: listItemDelegate.fullName
                font.pointSize: 15
                color: "#333"
                width: item.width - apparatusSelection.width - removeIcon.width - 130
                anchors.verticalCenter: delegateRow.verticalCenter
                elide: Text.ElideRight

            }

            StyleMGComboBox {
                id: apparatusSelection
                width: Math.max(180, item.width * 0.3)
                anchors.verticalCenter: delegateRow.verticalCenter
                model: listItemDelegate.apparatusList
                currentIndex: listItemDelegate.finalApparatusIndex

                onActivated: function(selectedIndex) {
                    const saved = GymnastSelectModel.sourceModel.setFinalApparatus(
                                    listItemDelegate.athleteId,
                                    listItemDelegate.eventId,
                                    selectedIndex)
                    if (!saved)
                        apparatusSelection.currentIndex = listItemDelegate.finalApparatusIndex
                }
            }

            // Show a delete button when the mouse is over the delegate
            Image {
                id: removeIcon
                objectName: "btnRemoveGymnast"
                source: removeMouseArea.pressed ? "qrc:images/delete_icon_pressed.png" : "qrc:images/delete_icon.png"
                anchors.verticalCenter: delegateRow.verticalCenter
                opacity: enabled ? 1 : 0.2

                MouseArea {
                    id: removeMouseArea
                    anchors.fill: parent
                    onClicked: GymnastSelectModel.sourceModel.removeItem(listItemDelegate.fullName)
                }
            }
        }
    }
}
