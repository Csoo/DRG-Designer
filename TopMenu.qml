import QtQuick 2.10
import QtQuick.Controls 2.4
import "./NordStyle"

Item{
    id: topMenu
    MenuBar {
        anchors.fill: parent

        delegate: MenuBarItem {
                id: menuBarItem

                height: 30
                contentItem: Text {
                    text: menuBarItem.text
                    font: menuBarItem.font
                    opacity: enabled ? 1.0 : 0.3
                    color: menuBarItem.highlighted ? "#ffffff" : Nord.frost
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 20
                    opacity: enabled ? 1 : 0.3
                    color: menuBarItem.highlighted ? Nord.highlight : "transparent"
                }
            }
        background: Rectangle {
            height: 30
            color: Nord.night
        }

        Menu {
            id: fileMenu
            title: qsTr("Fájl")
            Action {text: qsTr("Új üres HBCs") }
            Action {text: qsTr("Mások HBCs betöltése") }
            
            delegate: MenuItem {
                id: fileMenuItem
                implicitWidth: 200
                implicitHeight: 40

                contentItem: Text {
                    text: fileMenuItem.text
                    font: fileMenuItem.font
                    opacity: enabled ? 1.0 : 0.3
                    color: fileMenuItem.highlighted ? "#ffffff" : Nord.frost
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 170
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    color: fileMenuItem.highlighted ? Nord.highlight : "transparent"
                }
            }
            background: Rectangle {
                color: Nord.night
                implicitWidth: 170
                implicitHeight: 40 * fileMenu.count
                radius: 2
            }
        }

        Menu {
            id: editMenu
            title: qsTr("Szerkesztés")
            Action { text: qsTr("Elem eltávolítsa") }
            delegate: MenuItem {
                id: editMenuItem
                implicitWidth: 200
                implicitHeight: 40

                contentItem: Text {
                    text: editMenuItem.text
                    font: editMenuItem.font
                    opacity: enabled ? 1.0 : 0.3
                    color: editMenuItem.highlighted ? "#ffffff" : Nord.frost
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 170
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    color: editMenuItem.highlighted ? Nord.highlight : "transparent"
                }
            }
            background: Rectangle {
                color: Nord.night
                implicitWidth: 170
                implicitHeight: 40 * editMenu.count
                radius: 2
            }
        }
        Menu {
            id: helpMenu
            title: qsTr("Súgó")
            Action { text: qsTr("Használat") }
            Action { text: qsTr("Qt rendszerről") }
            delegate: MenuItem {
                id: helpMenuItem
                implicitWidth: 200
                implicitHeight: 40

                contentItem: Text {
                    text: helpMenuItem.text
                    font: helpMenuItem.font
                    opacity: enabled ? 1.0 : 0.3
                    color: helpMenuItem.highlighted ? "#ffffff" : Nord.frost
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 170
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    color: helpMenuItem.highlighted ? Nord.highlight : "transparent"
                }
            }
            background: Rectangle {
                color: Nord.night
                implicitWidth: 170
                implicitHeight: 40 * helpMenu.count
                radius: 2
            }
        }
    }
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
