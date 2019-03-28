import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.12

Item {
    id: bottomArea
    property int topWidth: parent.width - 50
    property int topHeight: parent.height / 2 - 50
    property int heightRider: 80*factSize
    property int numRank: Math.round(topHeight / heightRider) - 1

    property double factX: (root.width)/(1280.0)
    property double factY: getFactY()
    property double lastFactY: 0
    property double factSize: 0.1 + factX*factY
    property double textSize: 35*factSize

    function getFactY(){
        var value = (root.height)/(1024.0)
        if (value > 0.8)
            value = 0.8

        return value
    }

    Gradient {
        id: greenGradient
        GradientStop { position: 0.0; color: "#066006" }
        GradientStop { position: 0.25; color: "#033f03" }
        GradientStop { position: 0.5; color: "#032603" }
        GradientStop { position: 0.75; color: "#033f03" }
        GradientStop { position: 1.0; color: "#066006" }
    }

    Gradient {
        id: blueGradient
        GradientStop { position: 0.0; color: "#091b8c" }
        GradientStop { position: 0.25; color: "#040f4f" }
        GradientStop { position: 0.5; color: "#020726" }
        GradientStop { position: 0.75; color: "#040f4f" }
        GradientStop { position: 1.0; color: "#091b8c" }
    }

    function selectColor(value){
        if(value === "green")
            return greenGradient

        if(value === "blue")
            return blueGradient
    }

    function setViewPos(value){
        timelist.contentY = value*heightRider
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "black"
        opacity: 0.8
    }

    Rectangle {
        id: baseList
        anchors.centerIn: background
        width: topWidth
        height: heightRider*numRank + (numRank-1)*2
        color: "transparent"
//        Rectangle {
//            width: 150*numberRiders
//            height: 200
//            color: "white"
//            opacity: 0.5
//            border.width: 1
//            border.color: "black"
//        }

        Component {
            id: contactDelegate
            Item {
                width: topWidth
                height: heightRider

                Rectangle {
                    anchors.fill: parent
                    color: 'transparent'

                    Rectangle {
                        anchors.fill: parent
                        radius: 10
                        LinearGradient {
                            anchors.fill: parent
                            source: parent
                            start: Qt.point(0, 0)
                            end: Qt.point(0, heightRider)
                            gradient: selectColor(colorRider)
                        }
                    }

                    Row {
                        anchors.fill: parent
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 0

                        Rectangle {
                            color: 'transparent'
                            width: 8*heightRider/5
                            height: heightRider
                            Text {
                                anchors.centerIn: parent
                                text: position
                                color: "white"
                                font.family: "Helvetica"
                                font.pointSize: textSize
                                font.bold: true
                                font.italic: true
                            }
                        }

                        Rectangle {
                            color: 'transparent'
                            width: heightRider
                            height: heightRider
                            Image {
                                anchors.centerIn: parent
                                width: 3*heightRider/4
                                height: 3*heightRider/4
                                fillMode: Image.PreserveAspectFit
                                source: flag
                                smooth: true
                            }
                        }

                        Rectangle {
                            color: 'transparent'
                            width: topWidth - (diff_metrics.width + 40 + time_metrics.width + 40 + 13*heightRider/5)
                            height: heightRider
                            Text {
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 10
                                text: name
                                color: "white"
                                font.family: "Helvetica"
                                font.pointSize: textSize
                                font.bold: true
                                font.italic: true
                            }
                        }

                        Rectangle {
                            color: 'transparent'
                            width:  time_metrics.width + 40
                            height: heightRider
                            Text {
                                id: timeText
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 10
                                text: time
                                color: "white"
                                font.family: "Helvetica"
                                font.pointSize: textSize
                                font.bold: true
                                font.italic: true
                            }

                            TextMetrics {
                                id: time_metrics
                                font: timeText.font
                                text: "00:00.000"
                            }
                        }

                        Rectangle {
                            color: 'transparent'
                            width:  diff_metrics.width + 40
                            height: heightRider
                            Text {
                                id: diffText
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.rightMargin: 10
                                text: diff
                                color: "white"
                                font.family: "Helvetica"
                                font.pointSize: textSize
                                font.bold: true
                                font.italic: true
                            }

                            TextMetrics {
                                id: diff_metrics
                                font: diffText.font
                                text: "+00:00.000"
                            }
                        }
                    }
                }
            }
        }

        ListView {
            id: timelist
            anchors.fill: parent
            spacing: 2
            model: rankingWCModel
            delegate: contactDelegate
//            highlight: Rectangle {
//                width: topWidth
//                height: heightRider
//                color: "lightsteelblue"
//            }
            focus: true
            clip: true
//            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
        }
    }

//            states: [
//                State {
//                    when: (runTime > 2 && runTime <= 5 && topArea.time === "00:00.000")
//                    PropertyChanges { target: timeToStart; visible: true}
//                    PropertyChanges { target: redLightBack; visible: false}
//                    PropertyChanges { target: yellowLightBack; visible: false}
//                    PropertyChanges { target: greenLightBack; visible: false}
//                },
//                State {
//                    when: (runTime === 2 && topArea.time === "00:00.000")
//                    PropertyChanges { target: timeToStart; visible: true}
//                    PropertyChanges { target: redLightBack; visible: true}
//                    PropertyChanges { target: yellowLightBack; visible: false}
//                    PropertyChanges { target: greenLightBack; visible: false}
//                },
//                State {
//                    when: (runTime === 1 && topArea.time === "00:00.000")
//                    PropertyChanges { target: timeToStart; visible: true}
//                    PropertyChanges { target: redLightBack; visible: true}
//                    PropertyChanges { target: yellowLightBack; visible: true}
//                    PropertyChanges { target: greenLightBack; visible: false}
//                },
//                State {
//                    when: (runTime == 0 && topArea.time === "00:00.000")
//                    PropertyChanges { target: timeToStart; visible: true}
//                    PropertyChanges { target: redLightBack; visible: true}
//                    PropertyChanges { target: yellowLightBack; visible: true}
//                    PropertyChanges { target: greenLightBack; visible: true}
//                }
//            ]
}

