import QtQuick 2.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Universal 2.3
import TypeEnum 1.0
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
                    asynchronous: true
                    sourceComponent: Component {
                        Label {
                            anchors.fill: parent
                            anchors.topMargin: 40
                            padding: 14
                            text: qsTr("Válaszon ki egy elemet a szerkesztés megkezdéséhez!")
                        }
                    }
                }
                Connections {
                    target: detailsLoader.item
                    ignoreUnknownSignals: true
                    function onIcd11Selected(index){ graph.addNodes(postCoordModel.getItemIndexes(index)) }
                    function onGoToICDDetails() { bno11Button.enabled = true }
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
                text: qsTr("Szerkesztő")
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
                text: qsTr("BNO-11 vizualizáció")
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

            property var prevMouseX: 0

            hoverEnabled: true

            width: 6

            drag {
                target: parent
                threshold: 1
                axis: Drag.XAxis
                smoothed: false
            }
            onPressed: {
                prevMouseX = mouseX
            }
            onMouseXChanged: {
                if (drag.active) {
                    var newWidth = treeView.width + (mouseX - prevMouseX)
                    //treeView.width = newWidth < treeView.maxWidth ? newWidth > treeView.minWidth ? newWidth : treeView.minWidth : treeView.maxWidth
                    if (newWidth < treeView.maxWidth)
                        if (newWidth > treeView.minWidth)
                            treeView.width = newWidth
                        else
                            treeView.width = treeView.minWidth
                    else
                        treeView.width = treeView.maxWidth
                }
            }
            cursorShape: Qt.SplitHCursor
        }

        onDrgClicked: {
            bno11Button.enabled = false
            drg.setAttributesFromModel(index)
            detailsLoader.sourceComponent = undefined
            detailsLoader.source = "DRGDetails.qml"
            tabBar.currentIndex = 0
            //graph.addNodes(treeModel.getItemIndexes(index))
        }
        onChapterClicked: {
            bno11Button.enabled = false
            detailsLoader.sourceComponent = undefined
            detailsLoader.source = "Editor.qml"
            tabBar.currentIndex = 0

        }
        onIcdClicked: {
            detailsLoader.sourceComponent = undefined
            icd.fetchFromModel(index)
            detailsLoader.setSource("ICDSwipe.qml", { "itemIndex": index })
            console.log(icd.getCode())
//            if (icd.isApproved) {
//                bno11Button.enabled = true
//                detailsLoader.setSource("ICDDetails.qml", { "index": index })
//            } else {
//                detailsLoader.source = "BNODetails.qml"
//            }
            bno11Button.enabled = false
        }
        BusyIndicator {
            id: drgBrowserBusy
            implicitHeight: 50
            implicitWidth: 50
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            running: true
            Connections {
                target: qmlManager
                function onDrgBrowserReady() {
                    qmlManager.stopConnection(Type.CONTINUE)
                    drgBrowserBusy.running = false
                }
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
