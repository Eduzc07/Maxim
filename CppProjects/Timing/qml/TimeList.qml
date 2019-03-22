import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

Item {
    id: root
    width: 150*count()
    height: 150
    signal stop(var idx)

    ListModel {
        id: timeModel
        ListElement {
            numStar: 0
            time: "00:00.000"
            startTime: "00:00:00.000"
            endTime: "00:00:00.000"
            diffTime: "+00:00.000"
            show: false
            //Rider Info
            position: "1"
            name: "Bill Smith"
            categoria: "D"
            club: "saqras"
            home: "Cusco"
            number: "0"
        }
    }

    function count(){
        return timeModel.count
    }

    function clearList(){
        timeModel.clear()
    }

    function removeIdx(idx){
        timeModel.remove(idx)
    }

    function addList(newTime){
        timeModel.append(newTime)
//        timeModel.insert(0, newTime)
    }

    function getStartTime(idx){
        return timeModel.get(idx).startTime
    }

    function getStartMin(idx){
        var stringMinute = timeModel.get(idx).startTime.split(":")[1]
        var stTime = Number.fromLocaleString(stringMinute)
        return stTime
    }

    function getShow(idx){
        return timeModel.get(idx).show
    }

    function getInfo(idx){
        return timeModel.get(idx)
    }

    function setTime(idx, time){
        timeModel.setProperty(idx, "time", time)
    }

    function setShow(idx, val){
        timeModel.setProperty(idx, "show", val)
    }

    function setPosDiff(idx, position, diff){
        var pos = "(" + Number(position).toLocaleString() + ")"
        timeModel.setProperty(idx, "position", pos)
        timeModel.setProperty(idx, "diffTime", diff)
    }

    function stopTime(idx, etime){
//        var ctime = timeModel.get(idx).time
        timeModel.setProperty(idx, "endTime", etime)

        console.log ("idx: " + idx + " eTime--->" + etime)
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: 0.5
            border.width: 1
            border.color: "black"
        }

        Component {
            id: contactDelegate
            Item {
                width: 150
                height: 150

                Rectangle {
                    color: 'transparent'
                    anchors.fill: parent

                    Column {
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 0

                        Rectangle {
                            color: 'transparent'
                            width: 150
                            height: 50

                            Text {
                                anchors.centerIn: parent
                                text: number
                                font.family: "Helvetica"
                                font.pointSize: 22
                                font.bold: true
                            }
                        }

                        Rectangle {
                            color: 'transparent'
                            width: 150
                            height: 50
                            Text {
                                anchors.centerIn: parent
                                text: time
                                font.family: "Helvetica"
                                font.pointSize: 18
                            }
                        }

                        Rectangle {
                            color: 'transparent'
                            width: 150
                            height: 50
                            Button {
                                anchors.centerIn: parent
                                text: "Stop"
                                onClicked: stop(index)
                                enabled: time !== "00:00.000"
                            }
                        }
                    }
                }
            }
        }
    }

    ListView {
        id: timelist
        anchors.fill: parent
        model: timeModel
        delegate: contactDelegate
        highlight: Rectangle {
            width: 150
            height: 150
            color: "lightsteelblue"
            radius: 5
        }
        focus: true
        orientation: ListView.Horizontal
//            verticalLayoutDirection: ListView.BottomToTop
//            clip: true
    }
}
