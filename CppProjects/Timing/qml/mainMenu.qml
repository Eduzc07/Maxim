import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

import io.qt.examples.backend 1.0
import io.qt.readdata 1.0
import io.qt.processtime 1.0
import "."

Item {
    id: root
    width: 1200
    height: 800
    visible: true

    property var locale: Qt.locale()
    property int stInterval: 0
    property bool flagStart: false
    property bool flagDisplay: false
    property int m_iRider: 0

    property string finalTime: ""

    Component.onCompleted: {
        console.log ("Starting--->")
        console.log ("The date is: " + new Date().toLocaleDateString(Qt.locale("en_EU")))
        mainList.clearList()
        timeList.clearList()
        resultList.clearList()
        readdata.readFile()
    }

    function paddZeros(time){
        var s = time.toString();
        if (s.length < 2) {
            s = "0" + s;
        }
        return s
    }

    function runningTime(currentDate, currentMinute){
        finalTime = currentDate.toLocaleTimeString(root.locale,"hh:mm:ss.zzz");
        var runTimeMs = currentDate.toLocaleTimeString(root.locale,"ss.zzz");

        var min = 0
        var time = ""

        for(var id = 0; id < timeList.count();id++){
            min = Number.fromLocaleString(currentMinute) - timeList.getStartMin(id)
            min = (min < 0)?0:min
            time = paddZeros(min) + ":"+ runTimeMs

            if (timeList.getShow(id))
                timeList.setTime(id, time)
        }
    }

    Timer {
        id: cleanTime
        property int tIdx: 0
        interval: 5000
        repeat: false
        onTriggered: {
            var countList = timeList.count()

            timeList.removeIdx(tIdx)
            console.log ("cleanTime---> " + countList)
        }
    }

    Timer {
        interval: 100
        running: true
        repeat: true

        onTriggered: {
            var currentDate = new Date()
            var dateVal = currentDate.toLocaleTimeString(locale,"hh:mm:ss");
            var stringList = dateVal.split(":")

            fullTime.text = stringList[0] + "h" + stringList[1] + "m" + stringList[2] + "s"

            var dateString = currentDate.toLocaleTimeString(locale,"ss");
            var time = 60.0 - Number.fromLocaleString(dateString)   // d == 1234.56

            if (time == 5 && !flagStart){
                console.log ("Prepare Flag--->")
                flagStart = true
                if (mainList.getStartTime() === ""){
                    return
                }

                timeList.addList(
                            { "numStar": mainList.getPosition(),
                                "number": mainList.getNumber(),
                                "time": "00:00.000",
                                "startTime": mainList.getStartTime(),
                                "endTime": "00:00:00.000",
                                "diffTime": "",
                                "show": false,
                                //Rider Info
                                "position": "",
                                "name": mainList.getRiderInfo().name,
                                "categoria": mainList.getRiderInfo().categoria,
                                "club": mainList.getRiderInfo().club,
                                "home": mainList.getRiderInfo().home})
            }

            if (time == 60
                    && flagStart
                    && mainList.getStartTime() === ""){
                stInterval = 1
                //Remove empty space
                mainList.remove(0)
                flagStart = false
            }

            if (time == 60 && flagStart){
                console.log ("Start--->")
                console.log ("StartTime--->" + mainList.getStartTime())

                //Reset interval to star a new category
                if (mainList.getPosition() === 1){
                    stInterval = 0
                }

                flagStart = false
                flagDisplay = true
                //Erase current rider
                mainList.remove(0)

                //Increase Rider
                m_iRider++

                timeList.setShow(timeList.count()-1, true)
            }

            //Starting time
            time = (time == 60)?0:time
            startTime.text = "00:" + paddZeros(time)

            if (flagDisplay)
                runningTime(currentDate, stringList[1])
        }
    }

    ProcessTime {
        id: procTime
    }

    BackEnd {
        id: backend
    }

    ReadData {
        id: readdata
        onRiderChanged: {
            console.log ("newRider QML = " + nombre)
            mainList.addList(
                        {"position": posicion,
                        "name": nombre,
                        "categoria": categoria,
                        "club": club,
                        "home": procedencia,
                        "number": numero,
                        "start": partida})
        }
    }

    Rectangle {
        anchors.fill: parent
        objectName: "rect"
        color: "black"

        Image {
            anchors.fill: parent
            opacity: 0.8
            fillMode: Image.PreserveAspectCrop
            source: "images/background_dh.jpg"
        }
    }

    MainList{
        id: mainList
        anchors.top: butt.bottom
        anchors.left: root.left
        anchors.margins: 10
    }

    Text {
        id: fullTime
//        text: readdata.userName
        font.family: "Helvetica"
        font.pointSize: 18
        color: "white"
        anchors.top: root.top
        anchors.horizontalCenter: mainList.horizontalCenter
    }

    Text {
        id: startTime
        font.family: "Helvetica"
        font.pointSize: 18
        color: "white"
        anchors.top: butt.top
        anchors.left: butt.right
    }

    TimeList{
        id: timeList
        anchors.bottom: mainList.bottom
        anchors.left: mainList.right
        anchors.margins: 10
        onStop: {
            stopTime(idx, finalTime)
//            flagDisplay = false

//            var currentDate = new Date()
//            var dateVal = currentDate.toLocaleTimeString(locale,"hh:mm:ss.zzz");
//            console.log ("idx: " + idx + "  Time--->" + dateVal)

            cleanTime.tIdx = idx
            cleanTime.start()

            timeList.setShow(idx, false)

            if (resultList.count() === 0) {
                //Save result
                resultList.addList({"position": "1",
                                       "name": timeList.getInfo(idx).name,
                                       "categoria": timeList.getInfo(idx).categoria,
                                       "club": timeList.getInfo(idx).club,
                                       "home": timeList.getInfo(idx).home,
                                       "number": timeList.getInfo(idx).number,
                                       "startTime": timeList.getInfo(idx).startTime,
                                       "endTime": timeList.getInfo(idx).endTime,
                                       "chronoTime": timeList.getInfo(idx).time,
                                       "diffTime": "+00:00.000"})

                procTime.setTimeRef("00:" + timeList.getInfo(idx).time)
                console.log ("pos ---------> ")
                timeList.setPosDiff(idx, 1,"+00:00.000")
                return
            }

            var pos = procTime.getDiff("00:" + timeList.getInfo(idx).time) + 1
            console.log ("pos ---> " + pos + "-- "+ procTime.elapsed)

            timeList.setPosDiff(idx, pos, procTime.elapsed)

            //Save result
            resultList.insert(pos-1,{"position": "1",
                                   "name": timeList.getInfo(idx).name,
                                   "categoria": timeList.getInfo(idx).categoria,
                                   "club": timeList.getInfo(idx).club,
                                   "home": timeList.getInfo(idx).home,
                                   "number": timeList.getInfo(idx).number,
                                   "startTime": timeList.getInfo(idx).startTime,
                                   "endTime": timeList.getInfo(idx).endTime,
                                   "chronoTime": timeList.getInfo(idx).time,
                                   "diffTime": "+00:00.000"})


            for(var id = 0; id < resultList.count(); id++){
                var post = procTime.getPos("00:" + resultList.getInfo(id).chronoTime) + 1
                resultList.setPosition(id, post)
                resultList.setElapsed(id, procTime.elapsed)
            }
        }
    }

    Button {
        width: 100
        height: 40
        text: "Load"
        anchors.top: mainList.bottom
        anchors.horizontalCenter: mainList.horizontalCenter
        anchors.topMargin: 10
        enabled: false
        onClicked: {
            mainList.clearList()
            readdata.readFile()
        }
    }

    Button {
        id: butt
        width: 100
        height: 40
        text: "Button"
        anchors.top: fullTime.bottom
        anchors.horizontalCenter: fullTime.horizontalCenter
        anchors.topMargin: 10
        onClicked: {
            procTime.setTimeRef("00:00:25.767")

            var pos = procTime.getDiff("00:00:30.767")
            console.log ("pos ---> " + pos)

            pos = procTime.getDiff("00:00:20.767")
            console.log ("pos ---> " + pos)
        }
    }

    ResultList{
        id: resultList
        anchors.top: root.top
        anchors.right: root.right
        anchors.margins: 10
    }
}
