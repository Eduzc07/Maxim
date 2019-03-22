import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3

Item {
    id: root
    focus: true
    property bool updateTime: false
    property bool displayFirstRider: false
    property int runTime: 60

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

    signal emptyList()

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

    function setTime(value){
        if (riderTime.bDisplaying)
            return

        if (value === "")
            value = "00:00.000"
        riderTime.time = value
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


    Column {
        spacing: 0
        RiderTime {
            id: riderTime
            width: root.width
            height: root.height / 2
        }

        Rectangle {
            width: root.width
            height: root.height / 2
            color: "black"
            opacity: 0.8
        }
    }
}

