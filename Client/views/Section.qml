import QtQuick 2.0
import QtQuick.Controls 2.4
import "./NordStyle"

Rectangle {
    id: item1
    property string title: ""
    height: container.height
    color: Nord.softBackground
    Column {
        id: container
        width: parent.width
        spacing: 5
        Label {
            id: titleLabel
            text: item1.title
            anchors.left: parent.left
            anchors.right: parent.right
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            topPadding: 10
            font.bold: true
            font.pointSize: 14
            anchors.rightMargin: 0
            anchors.leftMargin: 14
            color: Nord.frost
        }
        Rectangle {
            height: 2
            color: Nord.accent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 14
            anchors.leftMargin: 14
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.6600000262260437;height:480;width:640}
}
##^##*/
