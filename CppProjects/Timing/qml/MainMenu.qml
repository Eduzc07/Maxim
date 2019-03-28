import QtQuick 2.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

import io.qt.examples.backend 1.0
import io.qt.readdata 1.0
import io.qt.processtime 1.0
import "."

Item {
    id: root
    anchors.fill: parent
    visible: true
    focus: true

    property var locale: Qt.locale()
    property int stInterval: 0
    property bool flagStart: false
    property bool flagDisplay: false
    property int m_iRider: 0
    property int m_timeInterval: 10

    property string finalTime: ""

    state: "LoadList"

    Component.onCompleted: {
        mainList.clearList()
        timeList.clearList()
        resultList.clearList()
        toolView.displayClean()
        readdata.runDefaults()

        var day = new Date().toLocaleString(Qt.locale("es_PE"), "dd")
        var dayName = new Date().toLocaleString(Qt.locale("es_PE"), "dddd")
        dayName = dayName[0].toLocaleUpperCase() + dayName.slice(1)
        var month = new Date().toLocaleString(Qt.locale("es_PE"), "MMMM")
        month = month[0].toLocaleUpperCase() + month.slice(1)
        var year = new Date().toLocaleString(Qt.locale("es_PE"), "yyyy")
        var date = "%1, %2 de %3 de %4"
        dateFullTime.text = date.arg(dayName).arg(day).arg(month).arg(year)
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            showNormal()
            event.accepted = true;
        }
        if (event.key === Qt.Key_F) {
            showFullScreen()
            event.accepted = true;
        }
        if (event.key === Qt.Key_Space) {
            if (timeList.count() !== 0
                && timeList.getInfo(0).time !== "00:00.000"){
                timeList.stop(0)
            }
            event.accepted = true;
        }
    }

    function showSubWindow(){
        //Display normal if there is only one Screen connected
        subWindow.showNormal()
    }

    function setStateResult(){

        toolView.displaLastRider()
        root.state = "Result"
        readdata.getCatComboBox()

        //Save Automatically
        readdata.saveResult("file://Result.csv")
        readdata.saveStartList("file://StartList.csv")

        resultList.defRank = parseInt((root.height - 420 )/40)
    }

    WorkerScript{
        id: myWorker
        source: "dataloader.mjs"
    }

    Window {
        id: subWindow
        width: 600
        height: 480
        minimumWidth: 600
        minimumHeight: 480
        flags: Qt.SubWindow
        visible: true

        Component.onCompleted: {
            //Display normal if there is only one Screen connected
            subWindow.showNormal()

            //Display Fullscreen in secondWindow if there is another Screen connected
            if (Qt.application.screens.length > 1){
                screen = Qt.application.screens[1]
                x = screen.virtualX + 200
                y = screen.virtualY + 50
                initTimer.start()
            }
        }

        Timer {
            id: initTimer
            interval: 500
            running: false
            repeat: false
            onTriggered: {
                //            subWindow.showMaximized()
                subWindow.showFullScreen()
                if (subWindow.visibility !== 5)
                    initTimer.start()
            }
        }

        Image {
            opacity: 0.8
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: "images/rider_6.jpg"
        }
        ToolView{
            id: toolView
            anchors.fill: parent
            onEmptyList: {
                timeList.updateRan = true
                displayRider(0)
            }
        }

        BarOption{
            anchors.top: parent.top
            anchors.right: parent.right
            onSetFullScreen: {
                if (subWindow.visibility !== 5)
                    subWindow.showFullScreen()
                else
                    subWindow.showMaximized()
            }
        }
    }

//    function paddZeros(time){
//        var s = time.toString();
//        if (s.length < 2) {
//            s = "0" + s;
//        }
//        return s
//    }

    function runningTime(currentDate){
        finalTime = currentDate.toLocaleTimeString(root.locale,"hh:mm:ss.zzz");
        for(var id = 0; id < timeList.count();id++){
            var time = procTime.getTime(finalTime, timeList.getStartTime(id))
            if (timeList.getShow(id))
                timeList.setTime(id, time)

            if (id == 0 && toolView.updateTime){
                toolView.setTime(time)

                    //Display result
//                    procTime.getColor(timeList.getInfo(0).time)
//                    toolView.setColor(procTime.color)

                //Display result
                var data = timeList.getInfo(0).time + ","
                data += timeList.getInfo(0).categoria
                readdata.getColorRider(data)
                toolView.setColor(readdata.color)
                toolView.setPos(readdata.ranking)

            }
        }
    }

//    function clean(){
//        var rider = {"backColor": "red",
//            "pos": "-",
//            "name": "Nombre",
//            "num": "#",
//            "club": "Club",
//            "home": "Proc.",
//            "cat": "Cat"}
//        toolView.setRider(rider)

////        var riderImage = "images/rider_4.jpg"
////        var riderFlag = "images/peru-flag.png"

//        var riderImage = ""
//        var riderFlag = ""

//        toolView.setRiderExtra(riderImage, riderFlag)
//        toolView.setTime("00:00.000")
//        toolView.setColor("gray")

//        var diffRider = {"diff": "+00:00.000",
//            "showDiff": false,
//            "pos": "-"}
//        toolView.setDiff(diffRider)
//    }

    function displayRider(idx) {
        if (idx === 0){
            toolView.updateTime = true;
        }

        if (timeList.count() > 0 && timeList.updateRan){

            if ((timeList.lastCategory !== timeList.getInfo(idx).categoria)
                && (timeList.currentCategory !== timeList.getInfo(idx).categoria))
                toolView.displayClean()

            var rank = {
                "position": "-",
                "flag": "images/peru-flag.png",
                "name": timeList.getInfo(idx).name,
                "time": "00:00.000",
                "diff": "",
                "colorRider": "green"}
            toolView.addRanking(idx, rank);
        }

        if (timeList.count() === 0){
            if (mainList.count() > 0)
                toolView.displayFirstRider = false
            return
        }

        var rider = {"backColor": "gray",
            "pos": "-",
            "name": timeList.getInfo(idx).name,
            "num": timeList.getInfo(idx).number,
            "club": timeList.getInfo(idx).club,
            "home": timeList.getInfo(idx).home,
            "cat": timeList.getInfo(idx).categoria}
        toolView.setRider(rider)

        var riderImage = "images/rider_5.jpg"
        var riderFlag = "images/peru-flag.png"
        toolView.setRiderExtra(riderImage, riderFlag)

        toolView.setTime("00:00.000")
        var diffRider = {"diff": "+00:00.000",
            "showDiff": false,
            "pos": "-"}
        toolView.setDiff(diffRider)

        toolView.displayFirstRider = true

    }

    function setRank(value, timeValue){
        mainList.defRank = value
        resultList.defRank = value
        timeList.defRank = timeValue
    }

    Timer {
        id: cleanTime
        property int tIdx: 0
        interval: 5000
        repeat: false
        onTriggered: {
            var countList = timeList.count()

            if (countList === 0 && mainList.count() > 0){
//                clean()
                toolView.displayFirstRider = false
            }

//            if (countList > 0)
//                displayRider(0)

            console.log ("cleanTime---> " + countList)
        }
    }

    Timer {
        id: mainTimer
        property bool bTimeStart: false
        property bool bRunTime: true
        property string nextStart: readdata.partida
        interval: 50
        running: true
        repeat: true

        onTriggered: {
            var currentDate = new Date()
            var dateVal = currentDate.toLocaleTimeString(locale,"hh:mm:ss");
            var stringList = dateVal.split(":")

            fullTime.text = stringList[0] + "h" + stringList[1] + "m" + stringList[2] + "s"

            var dateString = currentDate.toLocaleTimeString(locale,"ss");

            //Display running time
            if (flagDisplay)
                runningTime(currentDate)

            var time = procTime.getDiffSeg(dateVal, nextStart)
            //To display Start
            toolView.runTime = time
            //Start to display startTime
            if (time === 10 && !bTimeStart && mainList.count()!== 0){
                root.state = "Race"
                bTimeStart = true
            }

            if (!bTimeStart || !bRunTime)
                return

            if ((time === 5 || time === (m_timeInterval + 5))
                    && !flagStart){

                flagStart = true
                if (mainList.getStartTime() === "")
                    return

                if (mainList.count() === 0)
                    return

                console.log ("Prepare Flag--->")

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

                //AddRider to Display
                if (!toolView.displayFirstRider)
                    displayRider(0)
            }

            if ((time === 0 || time === m_timeInterval)
                    && flagStart
                    && mainList.getStartTime() === ""){
                stInterval = 1

                //Remove empty space
                mainList.remove(0)
                flagStart = false
            }

            if (time === 0 && flagStart){
                console.log ("Start--->")
                console.log ("StartTime--->" + mainList.getStartTime())

                flagStart = false
                flagDisplay = true

                //Increase Rider
                m_iRider++

                timeList.setShow(timeList.count()-1, true)

                //Reset interval to star a new category
                if (mainList.getPosition() === 1){
                    stInterval = 0
                }

                //Erase current rider
                mainList.remove(0)

                //Finish start
                if (mainList.count()===0){
                    startTime.text = ""
                    bRunTime = false
                    return
                }

                //Save Next Rider
                nextStart = mainList.getStartTime()
                if (nextStart === ""){
                    nextStart = mainList.getNextStartTime(1)
                }
            }

            //Display Start time
            startTime.text = procTime.getRunTime(nextStart, dateVal)
        }
    }

    ProcessTime {
        id: procTime
    }

    ReadData {
        id: readdata
        onRiderChanged: {
            mainList.addList(
                        {"position": posicion,
                            "name": nombre,
                            "categoria": categoria,
                            "club": club,
                            "home": procedencia,
                            "number": numero,
                            "start": partida})
        }
        onStartTimeChanged: {
            topLeftArea.startTime = readdata.partida
            topLeftArea.timeRider = readdata.invRider
            topLeftArea.timeCat = readdata.invCat

            bottomRighttArea.startTime = readdata.partida
            bottomRighttArea.timeRider = readdata.invRider
            bottomRighttArea.timeCat = readdata.invCat
        }

        onListRankingChanged: {
            if (task === "Result"){
                resultList.clearList()
                resultList.updateResult(task)
            }

            if (task === "CatMenu"){
                listMenu.clear()
                var msg = {'action': task,'list': readdata.listRanking, 'model': listMenu};
                myWorker.sendMessage(msg);

                timerMenu.start()
            }
        }
    }

    MainList{
        id: mainList
        anchors.top: topLeftArea.bottom
        anchors.horizontalCenter: topLeftArea.horizontalCenter
        anchors.margins: 30
    }

    ToolTime {
        id: topLeftArea
        width: root.width / 2
        height: 160
        anchors.top: root.top
        anchors.left: root.left
        enableB2: false
        nameButton1: "Abrir"
        nameButton2: "Guardar"
        onClickButton1: {
//            var currentDate = new Date()
//            var dateVal = currentDate.toLocaleTimeString(locale,"hh:mm:ss");
//            var time = procTime.getDiffSeg(dateVal, topLeftArea.startTime)
//            if (time < 60*10){
//                showMessage()
//                return
//            }

            fileOpen()
            bOpen = true

            //Prepare to start all
            root.state = "LoadList"
            mainTimer.bTimeStart = false
            mainTimer.bRunTime = true
            toolView.displayClean()
            resultList.defRank = parseInt((root.height - 540 )/40)
        }
        onClickButton2: {
            fileSave()
            bSave = true
        }

        onSignalMessageOk: readdata.runDefaults()
    }

    Rectangle {
        id: topRightArea
        width: root.width / 2
        height: 160
        anchors.top: root.top
        anchors.right: root.right
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.8
        }

        Column {
            anchors.centerIn: parent
            spacing: 20
            Text {
                id: dateFullTime
                anchors.horizontalCenter: parent.horizontalCenter
                text: "time"
                font.family: "Helvetica"
                font.pointSize: 28
                color: "white"
            }
            Text {
                id: fullTime
                //        text: readdata.userName
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Helvetica"
                font.pointSize: 32
                color: "white"
            }
        }
    }

    ToolTime {
        id: bottomRighttArea
        width: root.width / 2
        height: 160
        anchors.bottom: root.bottom
        anchors.right: root.right
        nameButton1: "Resultado"
        nameButton2: "Hora de Partida"
        onClickButton1: {
            fileSave()
            bResult = true
        }
        onClickButton2: {
            fileSave()
            bStartList = true
//            root.state = "LoadList"
        }
    }

    Text {
        id: startTime
        font.family: "Helvetica"
        font.pointSize: 18
        color: "white"
        anchors.top: mainList.top
        anchors.left: mainList.right
        anchors.topMargin: 90
        anchors.leftMargin: 10
    }

    TimeList {
        id: timeList
        focus: true
        visible: true
        property string lastCategory: ""
        property string currentCategory: ""
        property bool updateRan: true
        width: root.width - 10
        height: 250
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        anchors.margins: 5
        onStop: {
            stopTime(idx, finalTime) //Save in endTime
            timeList.setShow(idx, false)

            //StopTime in Display
            toolView.updateTime = false

            if (idx !== 0){
                if (updateRan)
                    toolView.deleteFirst()
                displayRider(idx)
                toolView.setTime(timeList.getInfo(idx).time)
                updateRan = false
            }

            //Send to cpp
            var data = timeList.getInfo(idx).number + ","
            data += timeList.getInfo(idx).time + ","
            data += timeList.getInfo(idx).categoria
            readdata.storageRider(data)

            //Display result
            toolView.setColor(readdata.color)
            var diffSecRider = {"diff": readdata.flatElapsed,
                "showDiff": true,
                "pos": readdata.ranking}
            toolView.setDiff(diffSecRider)

            toolView.add({"backColor": readdata.color,
                    "pos": readdata.ranking,
                    "name": timeList.getInfo(idx).name,
                    "num": timeList.getInfo(idx).number,
                    "club": timeList.getInfo(idx).club,
                    "home": timeList.getInfo(idx).home,
                    "cat": timeList.getInfo(idx).categoria,
                    "time": timeList.getInfo(idx).time,
                    "diff": readdata.flatElapsed,
                    "showDiff": true,
                    "riderImage": "images/rider_5.jpg",
                    "riderFlag": "images/peru-flag.png"})

            //Leave the last rider in window
            if (mainList.count() === 0 && timeList.count() === 1)
                setStateResult()

            if (lastCategory === timeList.getInfo(idx).categoria){
                timeList.removeIdx(idx)
                return
            }

            if (currentCategory !== timeList.getInfo(idx).categoria){
                lastCategory = currentCategory
                currentCategory = timeList.getInfo(idx).categoria
                resultList.clearList()
                procTime.cleanRanking()
            }

            if (resultList.count() === 0) {
                currentCategory = timeList.getInfo(idx).categoria
                //Save result
                resultList.addList({"position": "1",
                                       "name": timeList.getInfo(idx).name,
                                       "categoria": timeList.getInfo(idx).categoria,
                                       "club": timeList.getInfo(idx).club,
                                       "home": timeList.getInfo(idx).home,
                                       "number": timeList.getInfo(idx).number,
                                       "chronoTime": timeList.getInfo(idx).time,
                                       "diffTime": "+00:00.000"})

                toolView.setRankingDiff(diffSecRider)
                procTime.setTimeRef(timeList.getInfo(idx).time)
//                timeList.setPosDiff(idx, 1,"+00:00.000")
                timeList.removeIdx(idx)
                return
            }

            var pos = procTime.getDiff(timeList.getInfo(idx).time) + 1
//            timeList.setPosDiff(idx, pos, procTime.elapsed)

            //Save result
            resultList.insert(pos-1,{"position": "1",
                                  "name": timeList.getInfo(idx).name,
                                  "categoria": timeList.getInfo(idx).categoria,
                                  "club": timeList.getInfo(idx).club,
                                  "home": timeList.getInfo(idx).home,
                                  "number": timeList.getInfo(idx).number,
                                  "chronoTime": timeList.getInfo(idx).time,
                                  "diffTime": "+00:00.000"})

            for(var id = 0; id < resultList.count(); id++){
                var post = procTime.getPos(resultList.getInfo(id).chronoTime)
                resultList.setPosition(id, post)
                resultList.setElapsed(id, procTime.elapsed)
            }


            toolView.setRankingDiff(diffSecRider)

            timeList.removeIdx(idx)
        }

        onCancel: {
            var timeNSP = "59:00.000"//60 min
            timeList.setTime(idx, timeNSP)

            toolView.deleteFirst()

            //Send to cpp
            var data = timeList.getInfo(idx).number + ","
            data += timeList.getInfo(idx).time + ","
            data += timeList.getInfo(idx).categoria
            readdata.storageRider(data)

            //Leave the last rider in window
            if (mainList.count() === 0 && timeList.count() === 1){
                toolView.displaLastRider()
                root.state = "Result"
                readdata.getCatComboBox()
            }

            if (lastCategory === timeList.getInfo(idx).categoria){
                timeList.removeIdx(idx)
                return
            }

            if (currentCategory !== timeList.getInfo(idx).categoria){
                lastCategory = currentCategory
                currentCategory = timeList.getInfo(idx).categoria
                resultList.clearList()
                procTime.cleanRanking()
            }

            var pos = procTime.getDiff(timeList.getInfo(idx).time) + 1

            //Save result
            resultList.addList({"position": "-",
                                  "name": timeList.getInfo(idx).name,
                                  "categoria": timeList.getInfo(idx).categoria,
                                  "club": timeList.getInfo(idx).club,
                                  "home": timeList.getInfo(idx).home,
                                  "number": timeList.getInfo(idx).number,
                                  "chronoTime": "N.S.P.",
                                  "diffTime": ""})

            if (timeList.count() === 1){
                //Display result
                toolView.add({"backColor": "blue",
                        "pos": "-",
                        "name": timeList.getInfo(idx).name,
                        "num": timeList.getInfo(idx).number,
                        "club": timeList.getInfo(idx).club,
                        "home": timeList.getInfo(idx).home,
                        "cat": timeList.getInfo(idx).categoria,
                        "time": "N.S.P.",
                        "diff": readdata.flatElapsed,
                        "showDiff": false,
                        "riderImage": "images/rider_5.jpg",
                        "riderFlag": "images/peru-flag.png"})
            }

            timeList.removeIdx(idx)

            //Display next rider inmediatly when more are coming
            if (timeList.count() > 0)
                displayRider(0)

        }
    }

    ResultList {
        id: resultList
        anchors.top: topRightArea.bottom
        anchors.horizontalCenter: topRightArea.horizontalCenter
        anchors.margins: 30
    }

    ListModel {
        id: listMenu
        ListElement { key: "Categoria"; value: 0 }
    }

    ComboBox {
        id: comboBoxList
        anchors.right: resultList.right
        anchors.top: resultList.top
        anchors.margins: 5
        textRole: "key"
        visible: false
        model: listMenu
        onActivated: readdata.createCatResults(listMenu.get(currentIndex).value)

        Timer {
            id: timerMenu
            interval: 25
            running: false
            repeat: false
            onTriggered: {
                //Select the Last Category
                comboBoxList.currentIndex = listMenu.count - 2
            }
        }
    }

    states: [
        State {
            name: "LoadList"
            PropertyChanges { target: topLeftArea; enabled: true}
            PropertyChanges { target: bottomRighttArea; visible: false}
        },
        State {
            name: "Race"
            PropertyChanges { target: topLeftArea; enabled: false}
            PropertyChanges { target: bottomRighttArea; visible: false}
        },
        State {
            name: "Result"
//            PropertyChanges { target: topLeftArea; enabled: false}
            PropertyChanges { target: bottomRighttArea; visible: true}
            PropertyChanges { target: comboBoxList; visible: true}
            PropertyChanges { target: timeList; visible: false}
        }
    ]
}
