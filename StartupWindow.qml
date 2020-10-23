import QtQuick 2.0
import QtQuick.Controls 2.4
import "./NordStyle"

Item {
    anchors.fill: parent
    id: mainItem
    signal goToMain(int drgId)
    width: 600
    height: 400


    Component.onCompleted: {
        state = "initial"
        goToMain(1)
    }

    transitions: [
        Transition {
            from: "*"; to: "*"
            NumberAnimation {
                targets: userError
                property: "width"
                duration: 500
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: pwError
                property: "width"
                duration: 500
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: emptyError
                property: "width"
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }
    ]

    Rectangle {
        id: pwError
        color: Nord.error
        radius: 5
        border.width: 0
        anchors.verticalCenter: password.verticalCenter
        anchors.left: password.right
        anchors.verticalCenterOffset: 10
        anchors.leftMargin: 10
        width: 0
        height: pwErrorMsg.contentHeight + 6
        Label {
            id: pwErrorMsg
            maximumLineCount: 2
            anchors.fill: parent
            text: qsTr("Adja meg a jelszavát!")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            padding: 4
        }
    }

    Rectangle {
        id: userError
        color: Nord.error
        radius: 5
        border.width: 0
        anchors.verticalCenter: userName.verticalCenter
        anchors.left: userName.right
        anchors.verticalCenterOffset: 10
        anchors.leftMargin: 10
        width: 0
        height: userErrorrMsg.contentHeight + 6
        Label {
            id: userErrorrMsg
            anchors.fill: parent
            maximumLineCount: 2
            text: qsTr("Adja meg a felhasználónevét!")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            padding: 4
        }
    }

    Rectangle {
        id: emptyError
        color: mainItem.state == "authSuccess" ? Nord.ok : Nord.error
        radius: 5
        border.width: 0
        anchors.verticalCenter: userName.verticalCenter
        anchors.left: userName.right
        anchors.verticalCenterOffset: 30
        anchors.leftMargin: 10
        width: 0
        height: emptyErrorMsg.contentHeight + 6
        Label {
            id: emptyErrorMsg
            anchors.fill: parent
            maximumLineCount: 2
            text: mainItem.state == "authSuccess" ? "OK" : qsTr("Kitöltendő mezők")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            padding: 4
        }
    }

    Button {
        id: loadButton
        width: 220
        height: 33
        anchors.top: comboBox.top
        anchors.rightMargin: 50
        highlighted: true
        anchors.topMargin: 0
        text: qsTr("HBCs hierarchia betöltése")
        anchors.right: parent.right
        enabled: comboBox.currentIndex != 0 && mainItem.state == "authSuccess"
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
        enabled: mainItem.state == "authSuccess"
        text: qsTr("Új hierarchia létrehozása")
        anchors.right: parent.right
        onClicked: {
            goToMain(0)
        }
    }

    Button {
        id: closeButton
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
        y: 144
        width: 260
        height: 33
        anchors.left: parent.left
        anchors.bottom: closeButton.top
        anchors.bottomMargin: 30
        anchors.leftMargin: 50
        currentIndex: 0
        textRole: "title"
        valueRole: "id"
        model: listModel
        delegate: ItemDelegate {
            hoverEnabled: true
            height: comboBox.height
            width: comboBox.width
            background: Rectangle {
                anchors.fill: parent
                color: parent.highlighted ? Nord.accent : Nord.night
            }
            contentItem: Text {
                text: title
                verticalAlignment: Text.AlignVCenter
                color: Nord.frost
                font: comboBox.font
            }

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

    Item {
        id: userName
        x: 176
        width: 200
        height: 60
        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        TextField {
            id: userField
            height: 35
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            onPressed: {
                mainItem.state = "initial"
            }
        }

        Label {
            id: label
            text: qsTr("Felhasználó név")
            anchors.bottom: userField.top
            anchors.bottomMargin: 3
        }
    }

    Item {
        id: password
        x: 176
        width: 200
        height: 60
        anchors.top: userName.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        TextField {
            id: pwField
            height: 35
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            echoMode: TextInput.Password
            passwordCharacter: "*"
            onPressed: {
                mainItem.state = "initial"
            }
        }

        Label {
            id: label1
            x: 0
            y: 0
            text: qsTr("Jelszó")
            anchors.bottom: pwField.top
            anchors.bottomMargin: 3
        }
    }

    Button {
        id: authButton
        x: 200
        height: 30
        width: password.width
        text: qsTr("Azonosítás")
        highlighted: true
        anchors.top: password.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        enabled: mainItem.state != "authSuccess"
        onClicked: {
            if (pwField.text == "" && userField.text == "") mainItem.state = "empty"
            else if (pwField.text == "") mainItem.state = "emptyPw"
            else if (userField.text == "") mainItem.state = "emptyUser"
            else mainItem.state = "authSuccess"
        }
    }

    states: [
        State {
            name: "initial"
            PropertyChanges {
                target: userError
                visible: false
                width: 0
            }
            PropertyChanges {
                target: pwError
                visible: false
                width: 0
            }
            PropertyChanges {
                target: emptyError
                visible: false
                width: 0
            }
        },
        State {
            name: "emptyUser"
            PropertyChanges {
                target: userError
                explicit: true
                visible: true
                width: 180
            }
            PropertyChanges {
                target: pwError
                visible: false
                width: 0
            }
            PropertyChanges {
                target: emptyError
                visible: false
                width: 0
            }
        },
        State {
            name: "emptyPw"
            PropertyChanges {
                target: pwError
                visible: true
                width: 180
            }
            PropertyChanges {
                target: userError
                visible: false
                width: 0
            }
            PropertyChanges {
                target: emptyError
                visible: false
                width: 0
            }
        },
        State {
            name: "empty"
            PropertyChanges {
                target: emptyError
                visible: true
                width: 180
            }
            PropertyChanges {
                target: pwError
                visible: false
                width: 0
            }
            PropertyChanges {
                target: userError
                visible: false
                width: 0
            }
        },
        State {
            name: "authFailed"
            PropertyChanges {
                target: error
                width: 180
            }
        },
        State {
            name: "authSuccess"
            PropertyChanges {
                target: emptyError
                visible: true
                width: 50
            }
            PropertyChanges {
                target: pwError
                visible: false
                width: 0
            }
            PropertyChanges {
                target: userError
                visible: false
                width: 0
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
