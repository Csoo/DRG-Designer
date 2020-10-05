import QtQuick 2.3
import QtQuick.Controls 2.4
import "./NordStyle"

ApplicationWindow {
    id: applicationWindows
    visible: true
    width: 500
    height: 400
    title: qsTr("HBCs Tervező")
    menuBar: TopMenu {
        id: menu
        visible: false
    }

    Loader {
        id: windowLoader
        anchors.fill: parent
        source: "StartupWindow.qml"
    }

    StartupWindow {
        onGoToMain: {
            windowLoader.source = "MainWindow.qml"
            applicationWindows.visibility = "Maximized"
            menu.visible = true
            visible = false
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
