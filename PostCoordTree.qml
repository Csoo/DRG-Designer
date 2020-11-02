import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.3
import TypeEnum 1.0
import "./NordStyle"

TreeView {
    signal postCoordSelectionChanged(var selectedIndexes)

    backgroundVisible: false
    headerVisible: false
    frameVisible: false
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

    onClicked: {
        postCoordModel.loadChildren(postCoordModel.getId(index), 0, index)
        if (postCoordModel.getType(index) === Type.AXIS)
            selection.select(index, ItemSelectionModel.NoUpdate)
        else
            selection.select(index, ItemSelectionModel.Toggle)
        //sele
    }

    selection: ItemSelectionModel {
        model: postCoordModel
        onSelectionChanged: {
            postCoordSelectionChanged(selectedIndexes)
        }
    }
    itemDelegate: Text {
        text: styleData.value
        color: Nord.frost
        elide: Text.ElideNone
    }
    rowDelegate: Rectangle {
        id: rowBackground
        anchors.verticalCenter: parent.verticalCenter
        color: {
            if (postCoordModel.getType(postCoordModel.index(styleData.row, 0)) === Type.AXIS)
                return Nord.accent
            else
                return "transparent"
        }
    }

    selectionMode: SelectionMode.NoSelection

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

    TableViewColumn {
        role: "code"
        width: 100
    }
    TableViewColumn {
        role: "title"
        width: 400
    }
}
