import QtQuick 2.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Universal 2.3
import QuickQanava 2.0 as Qua
import "qrc:/QuickQanava" as Qan
import MyGraph 1.0
import "./NordStyle"

Item {
    id: item1
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
            anchors.fill: parent
            interactive: false
            currentIndex: tabBar.currentIndex
            Item {
                id: editor
                CustomGraph {
                    id: graph
                    anchors.fill: parent
                    Component.onCompleted: {
                        var n1 = graph.insertNode()
                        n1.label = "Hello World"
                    }
                }

            }
            Item {
                id: graphs
                Text {
                    id: asd
                    x: 231
                    y: 168
                    text: qsTr("HELLLO")
                }

                RadioButton {
                    id: radioButton
                    x: 403
                    y: 120
                    text: qsTr("Radio Button")
                }

                RadioButton {
                    id: radioButton1
                    x: 403
                    y: 168
                    text: qsTr("Radio Button")
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
                id: tabButton1
                text: qsTr("Details")
                rightPadding: 9
                leftPadding: 9
                padding: 3
                hoverEnabled: true
                font.pointSize: 14
                width: implicitWidth
                background:
                    Rectangle {
                        property bool isActive: false
                        color: tabButton1.checked ? Nord.background : Nord.softBackground
                    }
            }
            TabButton {
                id: tabButton2
                text: qsTr("Graph")
                rightPadding: 9
                leftPadding: 9
                padding: 3
                hoverEnabled: true
                font.pointSize: 14
                width: implicitWidth
                background:
                    Rectangle {
                        property bool isActive: false
                        color: tabButton2.checked ? Nord.background : Nord.softBackground
                    }
            }
        }
    }

    QQC1.TreeView {
        property int maxWidth: 800
        property int minWidth: 150
        id: treeView
        width: 200
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 30
        headerDelegate: Rectangle {
            width: 60
            height: 30
            color: "#f21111"
            radius: 1
            border.color: "#0052d9"
            border.width: 2
            Text {text: styleData.value ; elide: Text.ElideLeft; anchors.fill: parent;horizontalAlignment: Text.AlignLeft;verticalAlignment: Text.AlignTop ;minimumPixelSize: 5}
        }
        QQC1.TableViewColumn {
            title: "Name"
        }
        QQC1.TableViewColumn {
            title: "Permissions"
        }

        MouseArea {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: -3
            anchors.bottomMargin: 0
            anchors.topMargin: 0

            hoverEnabled: true

            width: 6

            drag {
                target: parent
                axis: Drag.XAxis
                minimumX: -1
                maximumX: 1
                smoothed: true
            }
            onMouseXChanged: {
                if (drag.active) {
                    var newWidth = mouseX + treeView.width
                    treeView.width = newWidth < treeView.maxWidth ? newWidth > treeView.minWidth ? newWidth : treeView.minWidth : treeView.maxWidth
                }
                treeView.x = 0
            }
            cursorShape: containsMouse ? Qt.SplitHCursor : Qt.ArrowCursor
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
