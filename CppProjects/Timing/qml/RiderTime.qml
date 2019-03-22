import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id: topArea
    property int topWidth: parent.width - 50
    property int topHeight: parent.height / 3 - 50
    property double factSize: 0.1 + (root.width*root.height)/(2*1219.0*498.0)

    property bool bDisplaying: false
    property bool bLastOne: false

    Component.onCompleted: riderDisplay.clear()
    signal ready()

    ListModel {
        id: riderDisplay
        ListElement {
            backColor: "yellow"
            pos: "1"
            name: "Bill Smith"
            num: "16"
            club: "saqras"
            home: "Cusco"
            cat: "D"
            time: "00:00:00"
            diff: "+00:00.000"
            showDiff: false
            riderImage: "images/rider_5.jpg"
            riderFlag: "images/peru-flag.png"
        }
    }


    property color backColor: "gray"
    property string pos: "-"
    property string name: "Nombre"
    property string num: "#"
    property string club: "Club"
    property string home: "Proc."
    property string cat: "Cat"

    property string time: "00:00.000"
    property string diff: "+00:00.100"
    property bool showDiff: false

    property string riderImage: ""
    property string riderFlag: ""
    property bool showHome: false

    Timer {
        id: cleanTime
        interval: 5000
        repeat: false
        onTriggered: {
            if (bLastOne && riderDisplay.count === 1)
                return

            riderDisplay.remove(0)
            if (riderDisplay.count > 0){
                displayRiders()
                cleanTime.start()
            } else {
                clean()
                emptyList()
                bDisplaying = false
            }
        }
    }

    function addList(rider){
        riderDisplay.append(rider)
        startDisplayingBuffer()
    }

    function startDisplayingBuffer(){
        bDisplaying = true
        displayRiders()
        if (riderDisplay.count > 0)
            cleanTime.start()
    }

    function displayRiders(){
        backColor = riderDisplay.get(0).backColor
        pos = riderDisplay.get(0).pos
        name = riderDisplay.get(0).name
        num = riderDisplay.get(0).num
        club = riderDisplay.get(0).club
        home = riderDisplay.get(0).home
        cat = riderDisplay.get(0).cat
        time = riderDisplay.get(0).time
        diff = riderDisplay.get(0).diff
        showDiff = riderDisplay.get(0).showDiff
        riderImage = riderDisplay.get(0).riderImage
        riderFlag = riderDisplay.get(0).riderFlag
        showHome = true
    }

    function clean(){
        backColor = "gray"
        pos = "-"
        name = "Nombre"
        num = "#"
        club = "Club"
        home = "Proc."
        cat = "Cat"
        time = "00:00.000"
        diff = "+00:00.000"
        showDiff = false
        riderImage = ""
        riderFlag = ""
        showHome = false
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Rectangle {
            anchors.fill: column
            color: topArea.backColor
            opacity: 0.8
        }

        Rectangle {
            anchors.fill: column
            color: "transparent"
            border.width: 1 + factSize
            border.color: "black"
        }

        Column {
            id: column
            anchors.centerIn: parent
            spacing: 0
            Row {
                spacing: 0
                Rectangle {
                    color: 'transparent'
                    width: 2*topArea.topWidth/7
                    height: topArea.topHeight
                    border.width: factSize
                    border.color: "black"

                    Image {
                        anchors.fill: parent
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectCrop
                        source: "images/wallpaperNumber.jpg"
                        smooth: true
                    }

                    Text {
                        anchors.centerIn: parent
                        text: topArea.pos
                        color: "white"
                        font.family: "Helvetica"
                        font.pointSize: 130*factSize
                        font.bold: true
                    }
                }

                Rectangle {
                    color: 'transparent'
                    width: topArea.topWidth/4.65
                    height: topArea.topHeight

                    Image {
                        anchors.fill: parent
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectCrop
                        source: topArea.riderImage
                        smooth: true
                    }

                    Rectangle {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        width: topArea.topWidth/4.65
                        height: topArea.topHeight / 5
                        color: 'black'
                        opacity: 0.8
                        visible: topArea.showHome

                        Text {
                            anchors.centerIn: parent
                            text: topArea.home
                            color: "white"
                            font.family: "Helvetica"
                            font.pointSize: 30*factSize
                            font.bold: true
                        }
                    }
                }

                Rectangle {
                    width: topArea.topWidth / 2
                    height: 2*topArea.topHeight / 3
                    color: "transparent"
                    border.width: 2 + factSize
                    border.color: "black"

                    Rectangle {
                        anchors.fill: parent
                        color: "black"
                        opacity: 0.6
                    }

                    Column {
                        Rectangle {
                            width: topArea.topWidth / 2
                            height: topArea.topHeight / 3
                            color: "transparent"
                            border.width: 1 + factSize
                            border.color: "black"
                            Row {
                                Rectangle {
                                    color: 'transparent'
                                    width: topArea.topWidth / 14
                                    height: topArea.topHeight / 3
                                    Image {
                                        anchors.centerIn: parent
                                        width: topArea.topWidth / 15
                                        height: topArea.topHeight / 8
                                        fillMode: Image.PreserveAspectFit
                                        source: topArea.riderFlag
                                        smooth: true
                                    }
                                }

                                Rectangle {
                                    color: 'transparent'
                                    width: 3*topArea.topWidth / 7
                                    height: topArea.topHeight / 3
                                    Text {
                                        anchors.centerIn: parent
                                        text: topArea.name
                                        color: "white"
                                        font.family: "Helvetica"
                                        font.pointSize: 38*factSize
                                    }
                                }
                            }
                        }

                        Row {
                            Rectangle {
                                color: 'transparent'
                                width: topArea.topWidth / 9
                                height: topArea.topHeight / 3
                                border.width: 1 + factSize
                                border.color: "black"
                                Text {
                                    anchors.centerIn: parent
                                    text: topArea.num
                                    color: "white"
                                    font.family: "Helvetica"
                                    font.pointSize: 50*factSize
                                }
                            }

                            Rectangle {
                                color: 'transparent'
                                width: 5*topArea.topWidth / 18
                                height: topArea.topHeight / 3
                                border.width: 1 + factSize
                                border.color: "black"
                                Text {
                                    anchors.centerIn: parent
                                    text: topArea.club
                                    color: "white"
                                    font.family: "Helvetica"
                                    font.pointSize: 35*factSize
                                }
                            }

                            Rectangle {
                                color: 'transparent'
                                width: topArea.topWidth / 9
                                height: topArea.topHeight / 3
                                border.width: 1 + factSize
                                border.color: "black"
                                Text {
                                    anchors.centerIn: parent
                                    text: topArea.cat
                                    color: "white"
                                    font.family: "Helvetica"
                                    font.pointSize: 35*factSize
                                }
                            }
                        }


                        Rectangle {
                            width: topArea.topWidth / 2
                            height: topArea.topHeight / 3
                            color: 'transparent'
                            visible: topArea.showDiff
                            Text {
                                anchors.centerIn: parent
                                text: topArea.diff
                                color: "white"
                                font.family: "Helvetica"
                                font.pointSize: 50*factSize
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: topArea.topWidth
                height: topArea.topHeight/3 + 40
                anchors.margins: 20
                color: 'transparent'
                Text {
                    anchors.centerIn: parent
                    text: topArea.time
                    color: "white"
                    font.family: "Helvetica"
                    font.pointSize: 80*factSize
                    font.bold: true
                }
            }

//            RowLayout {
//                width: topArea.topWidth
//                height: topArea.topHeight/3 + 40
//                anchors.margins: 20
//                spacing: 0

//                Rectangle {
//                    Layout.fillWidth: true
//                    Layout.preferredWidth: topArea.topWidth/2
//                    Layout.fillHeight: true
//                    Layout.alignment: Qt.AlignHCenter
//                    color: 'transparent'
////                        border.width: 1
////                        border.color: "black"
//                    Text {
//                        anchors.centerIn: parent
//                        text: topArea.time
//                        color: "white"
//                        font.family: "Helvetica"
//                        font.pointSize: 80*factSize
//                        font.bold: true
//                    }
//                }
//                Rectangle {
//                    Layout.fillWidth: true
//                    Layout.preferredWidth: topArea.topWidth/2
//                    Layout.fillHeight: true
//                    Layout.alignment: Qt.AlignHCenter
//                    color: 'transparent'
////                        border.width: 1
////                        border.color: "black"
//                    visible: topArea.showDiff
//                    Text {
//                        anchors.centerIn: parent
//                        text: topArea.diff
//                        color: "white"
//                        font.family: "Helvetica"
//                        font.pointSize: 70*factSize
//                    }
//                }
//            }
        }

        Rectangle {
            id: timeToStart
            width: topArea.topWidth/4
            height: topArea.topHeight/3 + 40
            property int rad: 70*factSize
            visible: false
            anchors.bottom: column.bottom
            anchors.left: column.left
            color: "transparent"
            opacity: 1

            Row {
                anchors.centerIn: parent
                spacing: 10
                Rectangle {
                    id: redLight
                    width: timeToStart.rad
                    height: width
                    radius: width/2
                    anchors.verticalCenter: parent.verticalCenter

                    color: "red"
                    opacity: 1
                    border.color: "red"
                    border.width: 1

                    Rectangle {
                        id: redLightBack
                        anchors.fill: parent
                        radius: width/2
                        color: "black"
                        opacity: 0.8
                        visible: true
                    }
                }

                Rectangle {
                    id: yellowLight
                    width: timeToStart.rad
                    height: width
                    radius: width/2
                    anchors.verticalCenter: parent.verticalCenter

                    color: "gold"
                    opacity: 0.8
                    border.color: "yellow"
                    border.width: 1

                    Rectangle {
                        id: yellowLightBack
                        anchors.fill: parent
                        radius: width/2
                        color: "black"
                        opacity: 0.8
                        visible: true
                    }
                }

                Rectangle {
                    id: greenLight
                    width: timeToStart.rad
                    height: width
                    radius: width/2
                    anchors.verticalCenter: parent.verticalCenter

                    color: "limegreen"
                    opacity: 1
                    border.color: "green"
                    border.width: 1

                    Rectangle {
                        id: greenLightBack
                        anchors.fill: parent
                        radius: width/2
                        color: "black"
                        opacity: 0.8
                        visible: true
                    }
                }
            }

            states: [
                State {
                    when: (runTime > 3 && runTime <= 5 && topArea.time === "00:00.000")
                    PropertyChanges { target: timeToStart; visible: true}
                },
                State {
                    when: (runTime === 3 && topArea.time === "00:00.000")
                    PropertyChanges { target: timeToStart; visible: true}
                    PropertyChanges { target: redLightBack; visible: false}
                    PropertyChanges { target: yellowLightBack; visible: true}
                    PropertyChanges { target: greenLightBack; visible: true}
                },
                State {
                    when: (runTime === 2 && topArea.time === "00:00.000")
                    PropertyChanges { target: timeToStart; visible: true}
                    PropertyChanges { target: redLightBack; visible: true}
                    PropertyChanges { target: yellowLightBack; visible: false}
                    PropertyChanges { target: greenLightBack; visible: true}
                },
                State {
                    when: (runTime == 1 && topArea.time === "00:00.000")
                    PropertyChanges { target: timeToStart; visible: true}
                    PropertyChanges { target: redLightBack; visible: true}
                    PropertyChanges { target: yellowLightBack; visible: true}
                    PropertyChanges { target: greenLightBack; visible: false}
                }
            ]
        }
    }
}
