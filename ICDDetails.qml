import QtQuick 2.0
import QtQuick.Controls 2.4
import "./NordStyle"

Item {
    id: item1
    property var index: undefined

    onIndexChanged: {
        postCoordModel.loadPostCoord(icd.id, 0)
    }

    PostCoordTree {
        id: postCoordTree
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.rightMargin: 50
        anchors.bottomMargin: 70
        anchors.topMargin: 60
        width: 250
        Component.onCompleted: {
           // postCoordModel
        }
    }
    Item {
        id: item2
        anchors.right: postCoordTree.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 40
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 60
        TextField {
            id: icdSearch
            width: 150
            onEditingFinished: {
                listModel.loadIcd11(text)
            }
        }
        ListView {
            anchors.left: parent.left
            anchors.top: icdSearch.bottom
            anchors.leftMargin: 0
            anchors.topMargin: 0
            height: 400
            model: listModel
            delegate: Rectangle {
                width: 240
                height: 30
                Text {
                    text: code + "  -   " + title
                }
            }
        }
    }
}
