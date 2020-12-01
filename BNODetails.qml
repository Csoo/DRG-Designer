import QtQuick 2.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Universal 2.3
import TypeEnum 1.0
import "./NordStyle"

Item {
    id: bno
    signal selectICD()

    function loadSuggestion() {
        qmlManager.stopConnection(Type.PAUSE)
        icd.loadRecommandation()
    }

    Keys.onReturnPressed: searchButton.activate()

    Section {
        id: searchSection
        title: "ICD-11 hozzárendelés"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: whoRecommedSection.top
        anchors.margins: 40
        anchors.bottomMargin: 20
        anchors.topMargin: 60
        Item {
            id: search
            anchors {
                right: parent.right
                left: parent.left
                top: parent.top
                margins: 14
            }
            height: 40
            anchors.topMargin: 60
            TextField {
                id: searcBar
                anchors.right: searchButton.left
                anchors.top: parent.top
                anchors.rightMargin: 2
                anchors.left: parent.left
                anchors.leftMargin: 0
                selectByMouse: true
            }
            Button {
                id: searchButton
                anchors.right: parent.right
                anchors.top: parent.top
                highlighted: true
                text: qsTr("Keresés")
                function activate() {
                    qmlManager.stopConnection(Type.PAUSE)
                    loadIndicator.running = true
                    listModel.searchIcd(searcBar.text)
                }
                onClicked: {
                    activate()
                }
            }
        }
        ListView {
            id: searchResult
            anchors.top: search.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 40
            snapMode: ListView.SnapToItem
            cacheBuffer: 150
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            anchors.topMargin: 20
            model: listModel
            currentIndex: -1
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
            delegate: Rectangle {
                property int idx: index
                width: parent.width
                height: 30
                color: {
                    if (idx == searchResult.currentIndex)
                        return Nord.accent
                    if (highlightZone.containsMouse)
                        return Nord.highlight
                    else
                        return Nord.background
                }
                MouseArea {
                    id: highlightZone
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        searchResult.currentIndex = searchResult.currentIndex == parent.idx ? -1 : parent.idx
                        if (searchResult.currentIndex != -1) {
                            icd.conceptType = conceptType
                            icd.title = title
                            icd.code = code
                            icd.id = id
                        } else {
                            icd.loadRecommandation()
                        }
                    }
                }
                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.25
                    text: code
                }
                Label {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.75
                    text: title
                    wrapMode: TextInput.WordWrap
                }
                Rectangle {
                    height: 1
                    width: parent.width
                    color: Nord.night
                    anchors.top: parent.bottom
                    anchors.topMargin: -1
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            BusyIndicator {
                id: loadIndicator
                anchors.fill: parent
                anchors.margins: 150
                running: false
                Connections {
                    target: qmlManager
                    function onSearchReady() {
                        loadIndicator.running = false
                        qmlManager.stopConnection(Type.CONTINUE)
                    }
                }
            }

        }
    }

    Section {
        id: whoRecommedSection
        property int id: 0
        property int conceptType: 0
        title: qsTr("WHO ajánlása")
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        height: 100
        anchors.margins: 40
        anchors.bottomMargin: 60
        Connections {
            target: icd
            function onIcdChanged() {
                bno.loadSuggestion()
            }
        }
        Label {
            id: suggestion
            anchors.fill: parent
            anchors.margins: 20
            anchors.topMargin: 50
            text: qsTr("Nincs ajánlás a jelenlegi kódhoz.")
            Component.onCompleted: {
                bno.loadSuggestion()
            }
        }
    }
    Connections {
        target: icd
        function onWhoSuggestionReady(noRecommendation) {
            qmlManager.stopConnection(Type.CONTINUE)
            if (!noRecommendation)
                suggestion.text = icd.getFullCode()
            selectIcdButton.noRecommendation = noRecommendation
        }
    }

    Button {
        id: selectIcdButton
        property bool noRecommendation: true
        highlighted: true
        anchors.right: parent.right
        anchors.top: whoRecommedSection.bottom
        anchors.margins: 20
        anchors.topMargin: 10
        anchors.rightMargin: 40
        text: searchResult.currentIndex == -1 ? qsTr("Ajánlás elfogadása") : qsTr("ICD-11 választása")
        enabled: searchResult.currentIndex != -1 || !noRecommendation
        onClicked: {
            selectICD()
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
