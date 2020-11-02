import QtQuick 2.3
import QtQuick.Controls 2.4
import "./NordStyle"

ApplicationWindow {
    id: applicationWindows
    visible: true
    width: 600
    height: 400
    title: qsTr("HBCs Tervező")
    //flags: Qt.FramelessWindowHint | Qt.Window

    Component.onCompleted: {
        treeModel.connectToDatabase()
        listModel.loadDrgs()
        busyIndicator.running = false
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

    BusyIndicator {
        id: busyIndicator
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 50
        anchors.horizontalCenter: parent.horizontalCenter
        running: false
    }

    StartupWindow {
        id: start
        onGoToMain: {
            if (treeModel.connectToDatabase()) {
                busyIndicator.running = true
                windowLoader.setSource("MainWindow.qml", {"drgId": drgId})
                busyIndicator.running = false
                applicationWindows.visibility = "Maximized"
                menu.visible = true
                start.visible = false
            } else {
                console.log(treeModel.dbError)
            }
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
