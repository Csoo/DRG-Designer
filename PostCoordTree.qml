import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.3
import TypeEnum 1.0
import "./NordStyle"

TreeView {
    id: postCoordTree
    signal postCoordSelectionChanged(var selectedIndexes)
    signal postCoordUpdated()

    backgroundVisible: false
    headerVisible: false
    frameVisible: false
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

    onClicked: {
        //postCoordModel.loadChildren(postCoordModel.getId(index), 0, index)
        if (postCoordModel.getType(index) === Type.AXIS || postCoordModel.getTitle(index) === "")
            selection.select(index, ItemSelectionModel.NoUpdate)
        else {
            selection.select(index, ItemSelectionModel.Toggle)
            icd.updatePostCoord(postCoordModel.getId(index), postCoordModel.getCode(index), postCoordModel.getTitle(index))
            postCoordUpdated()
        }
    }

    selection: ItemSelectionModel {
        model: postCoordModel
        onSelectionChanged: {
            postCoordSelectionChanged(selectedIndexes)
            postCoordModel.setSelectedIndexes(selectedIndexes)
        }
    }
    itemDelegate: Rectangle {
        id: itemBackground
        color: "transparent"
        Text {
            id: itemDelegate
            text: styleData.value
            color: Nord.frost
            elide: Text.ElideNone
        }
        Connections {
            target: postCoordTree
            function onClicked(index) {
                if (styleData.index === index) {
                    itemBackground.color = postCoordModel.isSelected(index) ? Nord.accent : "transparent"
                }
             }
        }
    }
    rowDelegate: Rectangle {
        id: rowBackground
        anchors.verticalCenter: parent.verticalCenter
        color: "transparent"
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

    onExpanded: {
        if (postCoordModel.isEmpty(index)) {
            postCoordModel.loadChildren(index)
        }
    }

    TableViewColumn {
        role: "code"
        width: 110
    }
    TableViewColumn {
        role: "title"
        width: 400
    }
}
