import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

import "."

Window {
    id: root
    width: 600
    height: 480
    visible: true
    screen: Qt.application.screens[0]

    Component.onCompleted: {
//        showMaximized()
        showFullScreen()
        console.log ("Starting--->")
        console.log ("The date is: " + new Date().toLocaleDateString(Qt.locale("en_EU")))
        initTimer.start()
    }

    Timer {
        id: initTimer
        interval: 500
        running: true
        repeat: false
        onTriggered: {
            mainArea.contentWidth = root.width
            mainArea.contentHeight = root.height
        }
    }

    Flickable {
        id: mainArea
        anchors.fill: parent
        boundsMovement: Flickable.StopAtBounds

        Image {
            id: background
            opacity: 0.8
            fillMode: Image.PreserveAspectCrop
            source: "images/background_dh.jpg"
        }

        MainMenu{
            id: mainMenu
            anchors.fill: parent
        }
    }

    BarOption{
        anchors.top: parent.top
        anchors.right: parent.right
        onSetFullScreen: {
            if (root.visibility !== 5)
                showFullScreen()
            else
                showMaximized()
        }
    }

    BarOption{
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 80
        sourceImage: "images/flag.png"
        onSetFullScreen: mainMenu.showSubWindow()
    }
}
