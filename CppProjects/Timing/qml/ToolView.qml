import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3

Item {
    id: root
    focus: true
    property bool updateTime: false
    property bool displayFirstRider: false
    property int runTime: 60
    property string wcCategory: ""
    property bool timeNewCatUpdate: true

    property int lastPos: 0

    signal emptyList()

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            subWindow.showNormal()
            event.accepted = true;
        }
        if (event.key === Qt.Key_F) {
            subWindow.showFullScreen()
            event.accepted = true;
        }
    }

    function setRider(rider){
        riderTime.backColor = rider.backColor
        riderTime.pos = rider.pos
        riderTime.name = rider.name
        riderTime.num = rider.num
        riderTime.club = rider.club
        riderTime.home = rider.home
        riderTime.cat = rider.cat
    }

    function setRiderExtra(image, flag){
        riderTime.riderImage = image
        riderTime.riderFlag = flag

        if (image === "")
            riderTime.showHome = false
        else
            riderTime.showHome = true
    }

    function setDiff(value){
        riderTime.diff = value.diff
        riderTime.showDiff = value.showDiff
        riderTime.pos = value.pos
    }

    function setRankingDiff(value, idx){
        if (riderTime.bDisplaying)
            return

        if ((wcCategory !== timeList.getInfo(idx).categoria))
            return

        if(lastPos > rankingWCModel.count)
            return

        rankingWCModel.setProperty(lastPos, "position", value.pos)
        rankingWCModel.setProperty(lastPos, "diff", value.diff)

        var id = 0
        if(lastPos == 0 && resultList.count()>0){
            procTime.setTimeRef(rankingWCModel.get(0).time)
            for(id = 0; id < rankingWCModel.count; id++){
                procTime.getPos(rankingWCModel.get(id).time)
                rankingWCModel.setProperty(id, "position", Number(id + 1).toLocaleString())
                rankingWCModel.setProperty(id, "diff", procTime.flatElapsed)
            }
        }else{
            for(id = lastPos; id < rankingWCModel.count; id++)
                rankingWCModel.setProperty(id, "position", Number(id + 1).toLocaleString())            
        }
    }

    function setTime(value){
        if (riderTime.bDisplaying)
            return

        if (value === "")
            value = "00:00.000"

        riderTime.time = value

        if (wcCategory !== timeList.getInfo(0).categoria)
            return

        rankingWCModel.setProperty(lastPos, "time", value)
    }

    function setColor(value){
        if (riderTime.bDisplaying)
            return

        riderTime.backColor = value
    }

    function add(value){
        riderTime.addList(value)
    }

    function displaLastRider(){
        riderTime.bLastOne = true
    }

    function displayClean(){
        riderTime.clean()
        rankingWCModel.clear()
    }

    function addRanking(idx, value){
        var data = timeList.getInfo(idx).time + ","
        data += timeList.getInfo(idx).categoria
        readdata.getColorRider(data)

        if ((wcCategory !== timeList.getInfo(idx).categoria) && timeNewCatUpdate)
            return

        //Request Current idx
        rankingWCModel.insert(readdata.ranking, value)
        lastPos = readdata.ranking
        timeNewCatUpdate = true

        rankingView.setViewPos(lastPos)
    }

    function checkRanking(){
        procTime.setTimeRef(rankingWCModel.get(0).time)
        for(var id = 0; id < rankingWCModel.count; id++){
            var post = procTime.getPos(rankingWCModel.get(id).time)
            rankingWCModel.setProperty(id, "position", Number(id + 1).toLocaleString())
            rankingWCModel.setProperty(id, "diff", procTime.flatElapsed)
            rankingWCModel.setProperty(id, "colorRider", "blue")
        }
    }

    function setPos(value){
        var vNum = parseInt(value)
        if (vNum === lastPos)
            return

        lastPos = parseInt(value)
//        if (vNum > rankingWCModel.count )
//            return
        if (wcCategory !== timeList.getInfo(0).categoria)
            return

        if(vNum === 0)
            return

        rankingWCModel.setProperty(vNum, "position", value)
        rankingWCModel.move(vNum - 1, vNum, 1)

        rankingView.setViewPos(vNum)
    }

    function deleteFirst(idx){
        if (wcCategory !== timeList.getInfo(idx).categoria)
            return

        rankingWCModel.remove(lastPos)
    }

    Column {
        spacing: 0
        RiderTime {
            id: riderTime
            width: root.width
            height: root.height / 2
            onReady: root.checkRanking()

            onRiderWaiting:{
                var idx = parseInt(rider.position) - 1
                //Request Current idx
                rankingWCModel.insert(idx, rider)

                checkRanking()
                rankingWCModel.setProperty(idx, "colorRider", "green")

                console.log("onRiderWaiting --->", rider.diff)

                for(var id = idx; id < rankingWCModel.count; id++)
                    rankingWCModel.setProperty(id, "position", Number(id + 1).toLocaleString())
            }
        }

        RankingView {
            id: rankingView
            width: root.width
            height: root.height / 2
        }
    }

    ListModel {
        id: rankingWCModel
        ListElement {
            position: "999"
            flag: "images/peru-flag.png"
            name: "Zarate E"
            time: "00:00.000"
            diff: ""
            colorRider: "green"
        }
        ListElement {
            position: "1"
            flag: "images/peru-flag.png"
            name: "Bruic L"
            time: "00:00.000"
            diff: ""
            colorRider: "blue"
        }
        ListElement {
            position: "2"
            flag: "images/peru-flag.png"
            name: "Brosnan T"
            time: "00:00.000"
            diff: "+00:02.012"
            colorRider: "blue"
        }
    }
}
