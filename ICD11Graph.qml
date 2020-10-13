import QtQuick 2.0
import QuickQanava 2.0 as Qua
import "qrc:/QuickQanava" as Qan
import MyGraph 1.0
import "./NordStyle"

Qan.GraphView {

    function addNode(title) {
        graph.clearGraph()
        var node = graph.insertNode()
        node.label = title
        node.item.x = 300
        node.item.y = 400
        //graph.selectedNodes(node)
    }

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
