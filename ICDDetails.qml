import QtQuick 2.0
import QtQuick.Controls 2.4
import TypeEnum 1.0
import "./NordStyle"

Item {
    id: item1
    property var index: undefined

    signal icd11Selected(var index)

    onIndexChanged: {
        postCoordModel.loadPostCoord(icd.id, 0)
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
        PostCoordTree {
            id: postCoordTree
            anchors{
                fill: parent
                topMargin: 60
                leftMargin: 14
            }
            Component.onCompleted: {
               // postCoordModel
            }
            onClicked: {
                if (postCoordModel.getType(index) === Type.AXIS) {
                    icd11Selected(index)
                }
            }
            onPostCoordSelectionChanged: {
                fullCode.text = searchBar.currentValue
                for (var i = 0; i < selectedIndexes.length; i++) {
                    var code = postCoordModel.getCode(selectedIndexes[i])
                    fullCode.text += (code.substr(0,1) === 'X' ? "&" : "/") + code
                }
            }
        }
    }
    Section {
        id: item2
        title: qsTr("ICD11")
        anchors.right: postCoordSection.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.leftMargin: 40
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 60
        ComboBox {
            Component.onCompleted: {
                listModel.clear()
            }
            id: searchBar
            anchors{
                top: parent.top
                left: parent.left
                topMargin: 60
                leftMargin: 14
            }
            model: listModel
            width: 350
            height: 28
            y: 50
            displayText: "BNO-11 keresés"
            editable: true
            textRole: "title"
            valueRole: "id"
            popup.y: searchBar.height
            onEditTextChanged: {
//                if (listModel.rowCount() > 0)
//                    popup.open()
            }
            onAccepted: {
                listModel.loadIcd11(searchBar.editText)
            }

            onCurrentValueChanged: {
                //item1.icd11Selected(currentValue)
                fullCode.text = currentValue
                postCoordModel.loadPostCoord(currentValue, 0)
            }

            indicator: Rectangle {
                x: searchBar.width - width;
                y: (searchBar.availableHeight - height) / 2;
                width: searchBar.height; height: searchBar.height; color: "red" }
            delegate: ItemDelegate {
                hoverEnabled: true
                height: searchBar.height
                width: searchBar.width
                background: Rectangle {
                    anchors.fill: parent
                    color: parent.highlighted ? Nord.accent : Nord.night
                }
                contentItem: Text {
                    text: title + " - " + code
                    verticalAlignment: Text.AlignVCenter
                    color: Nord.frost
                    font: searchBar.font
                }

                highlighted: searchBar.highlightedIndex === index
                MouseArea {
                    id: mouse
                    hoverEnabled: true
                    anchors.fill: parent
                    width: searchBar.width
                    onClicked: {
                        searchBar.currentIndex = index
                        searchBar.popup.close()
                    }
                }
             }
        }

        Label {
            id: fullCode
            anchors{
                top: searchBar.bottom
                left: parent.left
                topMargin: 40
                leftMargin: 14
            }
            x: 0
            y: 34
            width: 215
            height: 27
            text: "code"
        }
//        TextField {
//            id: icdSearch
//            width: 150
//            onEditingFinished: {
//                listModel.loadIcd11(text)
//            }
//        }
//        ListView {
//            anchors.left: parent.left
//            anchors.top: icdSearch.bottom
//            anchors.leftMargin: 0
//            anchors.topMargin: 0
//            height: 400
//            model: listModel
//            delegate: Rectangle {
//                width: 240
//                height: 30
//                Text {
//                    text: code + "  -   " + title
//                }
//            }
//        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
