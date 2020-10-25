import QtQuick 2.0
import QtQuick.Controls 2.4
import "./NordStyle"

Item {
    id: item1
    property string title: ""
    anchors.fill: parent
    Label {
        id: titleLabel
        text: parent.title
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        topPadding: 10
        font.bold: true
        font.pointSize: 14
        anchors.topMargin: 0
        anchors.rightMargin: 0
        anchors.leftMargin: 14
        color: Nord.frost
    }
    Rectangle {
        height: 2
        color: Nord.accent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleLabel.bottom
        anchors.topMargin: 4
        anchors.rightMargin: 14
        anchors.leftMargin: 14
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.6600000262260437;height:480;width:640}
}
##^##*/
