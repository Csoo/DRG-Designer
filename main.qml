import QtQuick 2.3
import QtQuick.Controls 2.4
import TypeEnum 1.0
import "./NordStyle"

ApplicationWindow {
    id: applicationWindows
    visible: true
    width: 600
    height: 400
    title: qsTr("HBCs Tervező")
    //flags: Qt.FramelessWindowHint | Qt.Window

    property bool connected: false

    onClosing: {
        qmlManager.stopConnection(Type.STOP)
    }

    menuBar: TopMenu {
        id: menu
        visible: false
    }

    Loader {
        id: windowLoader
        anchors.fill: parent
        source: ""
        onLoaded: {
            console.log(source, " is loaded")
        }
    }

    Connections {
        target: qmlManager
        function onConnectionReady(success, errorMsg) {
            if (errorMsg === "QUIT") {
                Qt.quit()
            }
            connected = success
            statusBar.dbError = !success
            statusTip.text = statusBar.dbError ? qsTr("Connection error!") + "/n" + errorMsg : qsTr("Adatbázis csatlakozva")
        }
    }

    StartupWindow {
        id: start
        onGoToMain: {
            windowLoader.setSource("MainWindow.qml", {"drgId": drgId})
            applicationWindows.visibility = "Maximized"
            menu.visible = true
            statusBar.visible = true
            start.visible = false
            listModel.clear()
            applicationWindows.minimumHeight = 500
            applicationWindows.minimumWidth = 900
        }
    }

    footer: ToolBar {
        id: statusBar
        property bool dbError: true
        position: ToolBar.Footer
        width: parent.width
        height: 20
        background: Rectangle {
            anchors.fill: parent
            color: Nord.night
        }
        Rectangle {
            anchors.right: parent.right
            width: 24
            height: parent.height
            color: statusBar.dbError ? Nord.error : Nord.ok
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onContainsMouseChanged: {
                    statusTip.visible = containsMouse
                }
            }
        }
    }

    ToolTip {
        id: statusTip
        delay: 500
        font.pixelSize: 12
        x: applicationWindows.width - 55
        y: applicationWindows.height - 50
        text: qsTr("Még nem sikerült csatlakotni az adatbázishoz.");
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
