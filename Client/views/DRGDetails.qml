import QtQuick 2.4
import QtQuick.Controls 2.4
import "./NordStyle"

Item {
    anchors.fill: parent
    ScrollView {
        anchors.fill: parent
        clip: true
        Flickable {
            contentHeight: hbcs.height + attributes.height + 100
            anchors.fill: parent
            anchors.bottomMargin: 60

            Section {
                id: hbcs
                title: qsTr("Homogén Betegség Csoport")
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.rightMargin: 40
                anchors.topMargin: 60
                height: hbcsForm.height + 70
                color: Nord.softBackground
                clip: true
                Grid {
                    id: hbcsForm
                    property int labelWidth: 200
                    property int valueWidth: 200
                    anchors.top: parent.top
                    verticalItemAlignment: Grid.AlignVCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 14
                    anchors.topMargin: 50
                    columns: 2
                    rowSpacing: 4
                    Label { text: qsTr("Főcsoport"); width: hbcsForm.labelWidth; height: 35 }
                    Label { text: drg.chapter ; width: hbcsForm.valueWidth; height: 35 }
                    Label { text: qsTr("Megnevezés"); width: hbcsForm.labelWidth; }
                    TextField { text: drg.title ; width: hbcsForm.valueWidth + 150; selectByMouse: true}
                    Label { text: qsTr("Csoport kód"); width: hbcsForm.labelWidth }
                    TextField { text: drg.code; width: hbcsForm.valueWidth; selectByMouse: true}
                }
            }

            Section {
                id: attributes
                title: qsTr("Attribútumok")
                anchors.top: hbcs.bottom
                anchors.horizontalCenter: hbcs.horizontalCenter
                anchors.topMargin: 30
                width: hbcs.width
                color: Nord.softBackground
                clip: true
                height: 340
                Column {
                    id: attributesForm
                    width: 300
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: 250
                    spacing: 15
                    anchors.topMargin: 72
                    TextField {
                        id: textField
                        height: 35
                        text: drg.simpleTitle
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0
                        selectByMouse: true
                    }

                    TextField {
                        id: textField1
                        height: 35
                        text: drg.weight
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0
                        selectByMouse: true
                    }

                    TextField {
                        id: textField2
                        height: 35
                        text: drg.minDay
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0
                        selectByMouse: true
                    }

                    TextField {
                        id: textField3
                        height: 35
                        text: drg.maxDay
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0
                        selectByMouse: true
                    }

                    TextField {
                        id: textField4
                        height: 35
                        text: drg.normativeDay
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0
                        selectByMouse: true
                    }

                }

                Column {
                    id: column
                    x: 57
                    width: 200
                    height: 400
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 72
                    anchors.leftMargin: 14
                    spacing: 15

                    Label {
                        id: label
                        height: 35
                        text: qsTr("Laikus megnevezés")
                        anchors.left: parent.left
                        anchors.right: parent.right
                        verticalAlignment: Text.AlignVCenter
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0
                    }

                    Label {
                        id: label1
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 35
                        text: qsTr("Súly")
                        verticalAlignment: Text.AlignVCenter
                        anchors.rightMargin: 0
                    }

                    Label {
                        id: label2
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 35
                        text: qsTr("Minimum ellátási napok száma")
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        anchors.rightMargin: 0
                    }

                    Label {
                        id: label3
                        width: 120
                        height: 35
                        text: qsTr("Maximum ellátási napok száma")
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        anchors.rightMargin: 0
                    }

                    Label {
                        id: label4
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 35
                        text: qsTr("Normális ellátási napok száma")
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.WordWrap
                        anchors.rightMargin: 0
                    }
                }

            }

            Row {
                anchors.right: attributes.right
                anchors.top: attributes.bottom
                anchors.rightMargin: 10
                anchors.topMargin: 20
                spacing: 5
                Button { id: resetButton; text: qsTr("Visszaállítás")}
                Button { id: saveButton; text: qsTr("Mentés"); highlighted: true }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
