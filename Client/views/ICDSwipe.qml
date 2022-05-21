import QtQuick 2.3
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Universal 2.3
import TypeEnum 1.0
import "./NordStyle"

Item {

    property var itemIndex: null

    signal goToICDDetails()

    onItemIndexChanged: {
        if (icd.isApproved)
            icdDetails.loadPostCoordinations()
    }
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: 0
        interactive: false

        onCurrentIndexChanged: {
            if (currentIndex === 1) {
                goToICDDetails()
            }
        }

        Component.onCompleted: {
            currentIndex = icd.isApproved ? 1 : 0
            if (currentIndex === 1) {
                goToICDDetails()
            }
        }
        BNODetails {
            onSelectICD: {
                swipeView.currentIndex = 1
                icd.clearPostCoord()
                icdDetails.loadPostCoordinations()
            }
        }
        ICDDetails {
            id: icdDetails
            modelIndex: itemIndex
            onChangeICD: {
                swipeView.currentIndex = 0
                icd.clearPostCoord()
            }
            Component.onDestruction: {
                icd.clearPostCoord()
            }
        }
    }

}
