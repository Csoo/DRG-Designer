import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQml.Models 2.3
import "./NordStyle"

TreeView {
    property int drgId: -1
    property int maxWidth: 1000
    property int minWidth: 300

    signal drgClicked(var index)
    signal chapterClicked(var index)
    signal icdClicked(var index)

    Component.onCompleted: {
        treeModel.loadDrgEntities(drgId)
    }
    id: treeView
    alternatingRowColors: true
    model: treeModel
    selection: ItemSelectionModel {
        model: treeModel
    }
    headerDelegate: Rectangle {
        width: 60
        height: 30
        color: "#f21111"
        radius: 1
        border.color: "#0052d9"
        border.width: 2
        Text {text: styleData.value ; elide: Text.ElideLeft; anchors.fill: parent;horizontalAlignment: Text.AlignLeft;verticalAlignment: Text.AlignTop ;minimumPixelSize: 5}
    }
    itemDelegate: Text {
        text: styleData.value
        color: styleData.textColor
        elide: styleData.elideMode
        wrapMode: Text.WordWrap
    }
    rowDelegate: Rectangle {
        id: rowBackground
        anchors.verticalCenter: parent.verticalCenter
        color: styleData.selected ? Nord.accent : "#fff"
        DropArea {
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: parent.height / 2
            width: parent.width
            height: 6
            onDropped: {
                console.log("dropped")
            }
            Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                height: 2
                width: parent.width
                color: Nord.accent
                visible: parent.containsDrag
            }
        }
    }

    onPressAndHold: {
        console.log("drag started: ", treeModel.data(index, 1))
        Drag.active = true
        Drag.source = treeView.rowDelegate
        Drag.startDrag()
        Drag.active = false
        console.log("drag ended")
    }

    onExpanded: {
        console.log("expanded")
        if (treeModel.isEmpty(index) && treeModel.depth(index) === 2) {
            console.log("icd expanded", treeModel.getId(index))
            treeModel.loadIcd(treeModel.getId(index), index)
            //treeModel.loadICD11(treeModel.data(index, 0))
        }
    }

    onClicked: {
        switch (treeModel.depth(index)) {
            case 1: chapterClicked(index); break;
            case 2: drgClicked(index); break;
            case 3: icdClicked(index); break;
        }
    }

    TableViewColumn {
        role: "code"
        title: "Code"
        width: 100
    }
    TableViewColumn {
        role: "title"
        title: "Title"
        width: 300
    }


}
