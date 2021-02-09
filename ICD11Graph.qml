import QtQuick 2.0
import QtQuick.Controls 2.4
import QuickQanava 2.0 as Qua
import "qrc:/QuickQanava" as Qan
import MyGraph 1.0
import "./NordStyle"

Item {
    id: graphContainer
    anchors.fill: parent
    function addNode(title) {
        graph.clearGraph()
        var node = graph.insertNode()
        node.label = title
        node.item.x = 300
        node.item.y = 400
    }

    function addNodes(idxArray) {
        graph.clearGraph()
        var yPos = 300
        var xPos = 300
        var inc = 70
        var nodeArray = []
        var parent
        for (var i = 0; i < idxArray.length; i++) {
            nodeArray.push(graph.insertNode())
            nodeArray[i].item.height = 90
            nodeArray[i].label = postCoordModel.getCode(idxArray[i]) + "\n" + postCoordModel.getTitle(idxArray[i])
            if (postCoordModel.getTitle(idxArray[i]) === "") {
                nodeArray[i].item.selectable = false
                nodeArray[i].item.height = 50
                nodeArray[i].label = postCoordModel.getCode(idxArray[i])
                if (postCoordModel.getCode(idxArray[i]) === "") {
                    nodeArray[i].item.height = 90
                    nodeArray[i].label = qsTr("További elemek betöltése")
                }
            }
            nodeArray[i].item.x = xPos
            nodeArray[i].item.y = yPos
            for (var j = 0; j < i; j++) {
                if (postCoordModel.isParent(idxArray[j], idxArray[i])) {
                    parent = nodeArray[j]
                    var edge = graph.insertEdge(nodeArray[j], nodeArray[i])
                }
                xPos = parent.item.x + 200
                yPos = parent.item.y
            }
            yPos += inc
            inc += 70
        }
    }

    Qan.GraphView {
        id: graphView
        anchors.fill: parent
        navigable: true

        resizeHandlerColor: Nord.frost
        resizeHandlerSize: "10x10"
        gridThickColor: Nord.softBackground
        grid: Qan.LineGrid{
          gridScale: 50
          gridMajor: 10
          gridWidth: 2
        }
        graph: CustomGraph {
            id: graph
            selectionDelegate: Rectangle {
                radius: 25
                color: "transparent"
                border.width: 3
                border.color: Nord.accent
            }
            connectorEdgeColor: Nord.accent
            Component.onCompleted: {
                defaultEdgeStyle.lineColor = Nord.accent
                defaultNodeStyle.backColor = Nord.softBackground
                defaultNodeStyle.backOpacity = 1
                defaultNodeStyle.backRadius = 20
                defaultNodeStyle.borderColor = Nord.frost
            }
            onNodeClicked: {
                if (node.item.label === "További elemek betöltése") {
                    newNode = graph.insertNode()
                    newNode.item.label = "Új elem"
                }
            }
        }
    }

    ComboBox {
        id: axisDropDown
        width: 200
        height: 25
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 40
        anchors.topMargin: 40
        model: axisModel
        textRole: "text"

        onActivated: {
            graphContainer.addNodes(postCoordModel.getItemIndexes(postCoordModel.getIndexOfBaseItem(currentIndex)))
        }
        delegate: ItemDelegate {
            hoverEnabled: true
            height: axisDropDown.height
            width: axisDropDown.width
            background: Rectangle {
                anchors.fill: parent
                color: parent.highlighted ? Nord.accent : Nord.night
            }
            contentItem: Text {
                text: text
                verticalAlignment: Text.AlignVCenter
                color: Nord.frost
                font: axisDropDown.font
            }

            highlighted: axisDropDown.highlightedIndex == index
        }
    }
    ListModel {
        id: axisModel
        ListElement {
            text: qsTr("Válasszon tengelyt..")
        }
    }
    Connections {
        target: postCoordModel
        function onPostCoordReady(numberOfAxis) {
            axisModel.clear()
            for (var i = 0; i < numberOfAxis; i++) {
                axisModel.append({ "text" : postCoordModel.getTitleOfBaseItem(i) })
            }
        }
    }

}



/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:640}D{i:4}
}
##^##*/
