import QtQuick

Item {
    id: listItemDelegate
    width: ListView.view ? ListView.view.width - 50 : 0
    height: 50

    BorderImage {
        id: item
        width: parent.width ; height: 50
        source: mouse.pressed ? "qrc:images/delegate_pressed.png" : "qrc:images/delegate.png"
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5

        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
//            onClicked: {
//                if (index !== -1 && _synced) {
//                    enginioModel.setProperty(index, "completed", !completed)
//                }
            }

        Row {
            spacing: 20
            anchors.fill: parent

            Rectangle { // just to create a space in the row
                color: "transparent"
                height: 50; width: 50
            }

            Text {
                id: regFirstNameText
                x: 50
                text: FirstName // role name
                font.pointSize: 15
                color: "#333"
                width: ((parent.width)/ 4) - 50
                anchors.verticalCenter: parent.verticalCenter

            }
            Text {
                id: regLastNameText
                text: LastName // role name
                font.pointSize: 15
                color: "#333"
                width: ((parent.width)/ 4) - 50
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: regCountryText
                text: Country // role name
                font.pointSize: 15
                color: "#333"
                width: ((parent.width)/ 4) - 50
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: regSexText
                text: Sex // role name
                font.pointSize: 15
                color: "#333"
                width: ((parent.width)/ 4) - 50
                anchors.verticalCenter: parent.verticalCenter
            }

            // Show a delete button when the mouse is over the delegate
            Image {
                id: removeIcon
                objectName: "btnRemoveGymnast"
                source: removeMouseArea.pressed ? "qrc:images/delete_icon_pressed.png" : "qrc:images/delete_icon.png"
                anchors.verticalCenter: parent.verticalCenter
                opacity: enabled ? 1 : 0.2

                MouseArea {
                    id: removeMouseArea
                    anchors.fill: parent
                    onClicked: GymnastDataModel.sourceModel.removeItem(FirstName, LastName)
                }
            }
        }
    }
}
