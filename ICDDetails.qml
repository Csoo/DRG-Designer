import QtQuick 2.0
import QtQuick.Controls 2.4
import TypeEnum 1.0
import "./NordStyle"

Item {
    id: item1

    property var modelIndex: null
    signal icd11Selected(var index)
    signal changeICD()

    function loadPostCoordinations() {
        if (icd.conceptType != -1) {
            postCoordBusy.running = true
            qmlManager.stopConnection(Type.PAUSE)
            postCoordModel.loadPostCoord(icd.id, icd.conceptType)
        }
    }

    Section {
        id: postCoordSection
        title: qsTr("Utó-koordinációk")
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.rightMargin: 50
        anchors.bottomMargin: 70
        anchors.topMargin: 60
        width: 350
        Label {
            id: noPostCoord
            anchors{
                fill: parent
                topMargin: 60
                leftMargin: 14
                rightMargin: 14
            }
            visible: false
            wrapMode: TextInput.WordWrap
            text: qsTr("Nem tásítható utó-koordináció a választott ICD-11 entitáshoz.")
        }
        PostCoordTree {
            id: postCoordTree
            anchors{
                fill: parent
                topMargin: 60
                leftMargin: 14
            }
            onClicked: {
                if (postCoordModel.getType(index) === Type.AXIS) {
                    icd11Selected(index)
                }
            }
            onPostCoordSelectionChanged: {
            }
            onPostCoordUpdated: {
                fullCode.text = icd.getFullCode()
            }
            BusyIndicator {
                id: postCoordBusy
                anchors.fill: parent
                anchors.margins: 120
                running: false
            }
            Connections {
                target: postCoordModel
                function onPostCoordReady(numberOfAxis) {
                    postCoordBusy.running = false
                    icd.loadDetails()
                    fullCode.text = icd.getFullCode()
                    noPostCoord.visible = numberOfAxis === 0
                    qmlManager.stopConnection(Type.CONTINUE)
                }
                function onLoadPostCoordTree() {
                    postCoordBusy.running = true
                }
            }
        }
    }

    Button {
        id: approveButton
        anchors.top: postCoordSection.bottom
        anchors.topMargin: 6
        anchors.right: postCoordSection.right
        text: qsTr("Mentés")
        highlighted: true
        onClicked: {
            if (!postCoordModel.isApproved(modelIndex))
                icd.approveICD(itemIndex)
        }
    }

    Section {
        id: icd11Details
        title: qsTr("ICD11")
        anchors.right: postCoordSection.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.leftMargin: 40
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 60
        anchors.bottomMargin: 14
        Column {
            id: details
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors{
                topMargin: 60
                leftMargin: 14
                rightMargin: 14
            }
            spacing: 15
            Label {
                id: code
                width: parent.width
                text: icd.code
            }
            Label {
                id: title
                width: parent.width
                text: icd.title
                wrapMode: TextInput.WordWrap
            }
            Label {
                id: description
                width: parent.width
                text: icd.description
                wrapMode: TextInput.WordWrap
            }
        }
        Column {
            id: postCoords
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: details.bottom
            anchors{
                topMargin: 20
                leftMargin: 14
                rightMargin: 14
            }
            Repeater {
                model: icd.postCoordCount
                Item {
                    height: 20
                    width: icd11Details.width
                    Label {
                        id: postCode
                        width: 80
                        text: icd.getPostCode(index)
                    }
                    Label {
                        id: postTitle
                        anchors.left: postCode.right
                        width: parent.width
                        text: icd.getPostTitle(index)
                        wrapMode: TextInput.WordWrap
                    }
                }
            }
        }
        Label {
            id: fullCode
            anchors.top: postCoords.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 14
            width: code.width
            text: ""
            wrapMode: TextInput.WordWrap
        }
        Button {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 20
            onClicked: {
                changeICD()
            }
            text: qsTr("ICD-11 változtatása")
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
