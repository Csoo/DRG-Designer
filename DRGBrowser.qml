import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4 as QQC2
import QtQml.Models 2.3
import TypeEnum 1.0
import "./NordStyle"

TreeView {
    property int drgId: -1
    property int maxWidth: 1000
    property int minWidth: 300

    signal drgClicked(var index)
    signal chapterClicked(var index)
    signal icdClicked(var index)
    signal typeClicked(var index)

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
        treeModel.getItemIndexes(index)
    }

    onExpanded: {
        console.log("expanded")
        if (treeModel.isEmpty(index) && treeModel.getType(index) === Type.ICD11) {
            console.log("icd expanded", treeModel.getId(index))
            treeModel.loadIcd(treeModel.getId(index), index)
        }

        if (treeModel.getType(index) === Type.DRG_CAPTER) {
            treeModel.setDrgAttributes(index)
        }
    }

    onClicked: {
        switch (treeModel.getType(index)) {
            case Type.DRG_CAPTER: chapterClicked(index); break;
            case Type.DRG: drgClicked(index); break;
            case Type.ICD11: icdClicked(index); break;
            case Type.DRG_TYPE: typeClicked(index); break;
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            contextMenu.isChapter = false
            contextMenu.isType = false
            var index = parent.indexAt(mouse.x, mouse.y)
            if (index.valid) {
                    switch (treeModel.getType(index)){
                    case Type.DRG_CAPTER : contextMenu.isChapter = true; break;
                    case Type.DRG_TYPE : contextMenu.isType = true; break;
                }
            } else {
                console.log("show context menu for other")
            }
            contextMenu.popup()
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


    QQC2.Menu {
        id: contextMenu
        property bool isChapter: false
        property bool isType: false
        QQC2.Action { text: "Uj HBCs hozzaadasa"; enabled: contextMenu.isChapter }
        QQC2.Action { text: "Uj ICD11 hozzaadasa"; enabled: contextMenu.isType }
        QQC2.Action { text: "Uj focsoport hozzaadasa" }

        delegate: QQC2.MenuItem {
            id: menuItem
            font.pixelSize: 10
            implicitWidth: 170
            implicitHeight: 28

            contentItem: Text {
                text: menuItem.text
                font: menuItem.font
                opacity: enabled ? 1.0 : 0.3
                color: menuItem.highlighted ? "#ffffff" : Nord.frost
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 170
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: menuItem.highlighted ? Nord.highlight : "transparent"
            }
        }
        background: Rectangle {
            color: Nord.night
            implicitWidth: 170
            implicitHeight: 28 * contextMenu.count
            radius: 2
        }
    }
}
