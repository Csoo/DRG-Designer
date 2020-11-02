import QtQuick 2.0
import QtQuick.Controls 2.4
import QuickQanava 2.0 as Qua
import "qrc:/QuickQanava" as Qan
import MyGraph 1.0
import "./NordStyle"

Item {
    id: item1
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
            nodeArray[i].label = treeModel.getCode(idxArray[i])
            nodeArray[i].item.x = xPos
            nodeArray[i].item.y = yPos
            for (var j = 0; j < i; j++) {
                if (treeModel.isParent(idxArray[j], idxArray[i])) {
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

        resizeHandlerColor: Nord.softBackground
        resizeHandlerSize: "5x5"
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
                var n1 = graph.insertNode()
                var n2 = graph.insertNode()
                n1.item.x = 100
                n1.item.y = 200
                n1.item.x = 100
                n1.item.y = 300
                n1.label = "Hello World"
                n2.label = "Igen"
                var e1 = graph.insertEdge(n1, n2)
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
        textRole: "title"
        valueRole: "id"
        model: listModel
        delegate: ItemDelegate {
            hoverEnabled: true
            height: axisDropDown.height
            width: axisDropDown.width
            background: Rectangle {
                anchors.fill: parent
                color: parent.highlighted ? Nord.accent : Nord.night
            }
            contentItem: Text {
                text: title
                verticalAlignment: Text.AlignVCenter
                color: Nord.frost
                font: axisDropDown.font
            }

            highlighted: axisDropDown.highlightedIndex == index
        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:640}D{i:4}
}
##^##*/
