import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.3
import TypeEnum 1.0
import "./NordStyle"

TreeView {
    backgroundVisible: false
    headerVisible: false
    frameVisible: false

    itemDelegate: Text {
        text: styleData.value
        color: Nord.frost
        elide: styleData.elideMode
    }
    rowDelegate: Rectangle {
        id: rowBackground
        anchors.verticalCenter: parent.verticalCenter
        color: styleData.selected ? Nord.accent : "transparent"
    }

    model: postCoordModel

    style: TreeViewStyle {
        branchDelegate: Item {
            width: indentation
            height: 16
            Text {
                visible: styleData.column === 0 && styleData.hasChildren
                text: styleData.isExpanded ? "-" : "+"
                color: Nord.accent
                font.bold: true
                font.pointSize: 10
                renderType: Text.NativeRendering
                style: Text.PlainText
                anchors.centerIn: parent
                anchors.verticalCenterOffset: 2
            }
        }
    }

    contentHeader: Label {
            text: qsTr("Utókoordinációk")
            elide: Text.ElideLeft
            font.pointSize: 12
            bottomPadding: 14
            color: Nord.accent
    }

    TableViewColumn {
        role: "code"
        width: 100
    }
    TableViewColumn {
        role: "title"
        width: 300
    }
}
