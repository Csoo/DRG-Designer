import QtQuick 2.0
import QtQuick.Controls 2.4
import "./NordStyle"

Item {
    id: item1
    anchors.fill: parent
    Column {
        id: column1
        width: 288
        height: 215
        anchors.left: parent.horizontalCenter
        anchors.top: parent.top
        anchors.leftMargin: -70
        spacing: 15
        anchors.topMargin: 72
        TextField {
            id: textField
            height: 35
            text: qsTr("Text Field")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
        }

        TextField {
            id: textField1
            height: 35
            text: qsTr("Text Field")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
        }

        TextField {
            id: textField2
            height: 35
            text: qsTr("Text Field")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
        }

        TextField {
            id: textField3
            height: 35
            text: qsTr("Text Field")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
        }

        TextField {
            id: textField4
            height: 35
            text: qsTr("Text Field")
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
        }

    }

    Column {
        id: column
        x: 57
        width: 149
        height: 400
        anchors.right: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 72
        anchors.rightMargin: 114
        spacing: 15

        Label {
            id: label
            height: 35
            text: qsTr("Megnevezés")
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
            text: qsTr("Kód")
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
            text: qsTr("Normáli ellátási napok száma")
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            anchors.rightMargin: 0
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1}D{i:8}
}
##^##*/
