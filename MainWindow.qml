import QtQuick 2.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Universal 2.3
import "./NordStyle"

Item {
    property int drgId: -1

    Item {
        anchors.left: treeView.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 30
        anchors.leftMargin: 0

        SwipeView {
            id: swipeView
            anchors.fill: parent
            interactive: false
            currentIndex: tabBar.currentIndex

            Item {
                id: editorTab
                Loader {
                    id: detailsLoader
                    anchors.fill: parent
                    source: "DRGDetails.qml"
                }
            }
            Item {
                id: graphTab
                ICD11Graph {
                    id: graph
                }
            }
        }

        TabBar {
            id: tabBar
            width: parent.width
            leftPadding: 25
            currentIndex: 0
            spacing: 2
            background: Rectangle {
                color: Nord.softBackground

            }
            TabButton {
                id: detailsButton
                text: qsTr("Részletek")
                rightPadding: 9
                leftPadding: 9
                padding: 3
                hoverEnabled: true
                font.pointSize: 14
                width: implicitWidth
                background:
                    Rectangle {
                        color: detailsButton.checked ? Nord.background : Nord.softBackground
                    }
            }
            TabButton {
                id: bno11Button
                enabled: false
                text: qsTr("BNO-11")
                rightPadding: 9
                leftPadding: 9
                padding: 3
                hoverEnabled: true
                font.pointSize: 14
                width: implicitWidth
                background:
                    Rectangle {
                        color: bno11Button.checked ? Nord.background : Nord.softBackground
                    }
            }
        }
    }

    DRGBrowser {
        id: treeView
        drgId: parent.drgId
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 30
        width: 350
        MouseArea {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.right
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            hoverEnabled: true

            width: 6

            drag {
                target: parent
                threshold: 1
                axis: Drag.XAxis
                smoothed: false
            }
            onMouseXChanged: {
                if (drag.active) {
                    var newWidth = mouseX + treeView.width + treeView.width - 6
                    treeView.width = newWidth < treeView.maxWidth ? newWidth > treeView.minWidth ? newWidth : treeView.minWidth : treeView.maxWidth
                }
                treeView.x = 0
            }
            cursorShape: containsMouse ? Qt.SplitHCursor : Qt.ArrowCursor
        }

        onDrgClicked: {
            bno11Button.enabled = false
            drg.setAttributesFromModel(index)
            detailsLoader.source = "DRGDetails.qml"
            tabBar.currentIndex = 0
        }
        onChapterClicked: {
            bno11Button.enabled = false
            detailsLoader.source = "Editor.qml"
            tabBar.currentIndex = 0

        }
        onIcdClicked: {
            icd.setAttributesFromModel(index)
            bno11Button.enabled = true
            detailsLoader.source = "Editor.qml"
            graph.addNode(icd.getCode())
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
