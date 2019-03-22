import QtQuick 2.12
import QtQuick.Controls 2.2

Item {
    id: root
    width: 60
    height: 60
    anchors.margins: 10
    property string sourceImage: "images/fullscreen.png"

    signal setFullScreen()

    Rectangle {
        id: backIcon
        anchors.fill: parent
        radius: 10
        color: "transparent"
        state: "hide"

        Image {
            id: imageIcon
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: sourceImage
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: setFullScreen()
            onEntered: backIcon.state = "show"
            onExited: backIcon.state = "hide"
        }

        states: [
            State {
                name: "show"
                PropertyChanges { target: imageIcon; visible: true}
            },
            State {
                name: "hide"
                PropertyChanges { target: imageIcon; visible: false}
            }
        ]

        transitions: [
            Transition {
                from: "show"; to: "hide"
                PropertyAnimation { target: backIcon
                    properties: "opacity"; duration: 200 }
            },
            Transition {
                from: "hide"; to: "show"
                PropertyAnimation { target: backIcon
                    properties: "opacity"; duration: 200 }
            } ]
    }
}
