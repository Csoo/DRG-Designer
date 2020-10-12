import QtQuick 2.0
import QtQuick.Controls 2.4
import "./NordStyle"

Item {
    anchors.fill: parent
    id: item1
    signal goToMain(int drgId)
    Button {
        width: 220
        height: 33
        anchors.top: comboBox.top
        anchors.rightMargin: 50
        highlighted: true
        anchors.topMargin: 0
        text: qsTr("HBCs hierarchia betöltése")
        anchors.right: parent.right
        enabled: comboBox.currentIndex != 0
        onClicked: {
            goToMain(comboBox.currentValue)
        }
    }

    Button {
        y: 397
        width: 220
        height: 33
        anchors.bottom: parent.bottom
        anchors.rightMargin: 50
        highlighted: true
        anchors.bottomMargin: 50
        text: qsTr("Új hierarchia létrehozása")
        anchors.right: parent.right
        onClicked: {
            goToMain(0)
        }
    }

    Button {
        y: 397
        width: 100
        height: 33
        text: qsTr("Bezárás")
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 50
        anchors.bottomMargin: 50
        onClicked: {
            Qt.quit()
        }
    }

    ComboBox {
        id: comboBox
        width: 260
        height: 33
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 50
        anchors.topMargin: 50
        currentIndex: 0
        onActivated: {

        }
        textRole: "title"
        valueRole: "id"
        model: listModel
        delegate: ItemDelegate {
            hoverEnabled: true
            height: comboBox.height
            width: comboBox.width
            contentItem: Text {
                text: title
                verticalAlignment: Text.AlignVCenter
                color: Nord.frost
                font: comboBox.font
            }
//            background: Rectangle {
//                anchors.fill: parent
//                color: comboBox.highlightedIndex == index ? Nord.highlight : Nord.softBackground
//            }

            highlighted: comboBox.highlightedIndex == index
            MouseArea {
                id: mouse
                hoverEnabled: true
                anchors.fill: parent
                width: comboBox.width
                onClicked: {
                    comboBox.currentIndex = index
                    comboBox.popup.close()
                }
            }
            ToolTip {
                contentHeight: 22
                padding: 3
                delay: 900
                visible: parent.hovered && code != ""
                x: mouse.mouseX
                y: mouse.mouseY - contentHeight
                text: "Utolsó módosítás: " + code
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:640}D{i:2}D{i:4}
}
##^##*/
