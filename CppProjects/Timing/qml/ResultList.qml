import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

Item{
    id: root
    property int riderHeight: 40
    property int defRank: 5
    property int topRiders: {return count()>defRank?defRank:count()}

    width: 540 + riderHeight;
    height: riderHeight + riderHeight*defRank + title.height

    function updateResult(task){
        var msg = {'action': task,'list': readdata.listRanking, 'model': riderResultModel};
        myWorker.sendMessage(msg);
    }

    function count(){
        return riderResultModel.count
    }

    function clearList(){
        riderResultModel.clear()
    }

    function addList(riderResult){
        riderResultModel.append(riderResult)
    }

    function insert(idx, riderResult){
        riderResultModel.insert(idx, riderResult)
    }

    function setPosition(idx, position){
        var pos = "0"
        if (position === 999999)
            pos = "-"
        else
            pos = Number(position).toLocaleString()

        riderResultModel.setProperty(idx, "position", pos)
    }

    function setElapsed(idx, elapsed){
        riderResultModel.setProperty(idx, "diffTime", elapsed)
    }

    function getInfo(idx){
        return riderResultModel.get(idx)
    }

    function getData(idx){
        var pos = riderResultModel.get(idx).position
        var name = riderResultModel.get(idx).name
        var categoria = riderResultModel.get(idx).categoria
        var club = riderResultModel.get(idx).club
        var home = riderResultModel.get(idx).home
        var number = riderResultModel.get(idx).number
        var chrono = riderResultModel.get(idx).chronoTime
        var diff = riderResultModel.get(idx).diffTime

        var data = number + ","
        data += chrono + ","
        data += diff
        return data
    }

    Rectangle {
        id: tittle
        width: root.width
        height: 50
        color: "transparent"

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.5
        }
        Text {
            anchors.centerIn: parent
            font.family: "Helvetica"
            font.pointSize: 20
            font.underline: true
            color: "white"
            text: "Lista de Llegada"
        }
    }

    Rectangle {
        id: header
        anchors.top: tittle.bottom
        anchors.horizontalCenter: tittle.horizontalCenter
        width: root.width
        height: riderHeight
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: 0.9
        }
        RowLayout{
            anchors.fill: parent
            spacing: 0
            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 60
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Lugar"
                    font.family: "Helvetica"
                    font.pointSize: 12
                    font.bold: true
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 180
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: {
                        switch (Qt.platform.os) {
                        case "linux": return -5
                        case "windows": return 0
                        }
                    }
                    Text {
                        Layout.leftMargin: 10
                        text: "Nombre"
                        font.family: "Helvetica"
                        font.pointSize: 12
                        font.bold: true
                    }
                    Text {
                        Layout.leftMargin: 10
                        text: "Club"
                        font.family: "Helvetica"
                        font.pointSize: 10
                        font.bold: true
                    }
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: riderHeight
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Cat"
                    font.family: "Helvetica"
                    font.pointSize: 12
                    font.bold: true
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 60
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Num"
                    font.family: "Helvetica"
                    font.pointSize: 12
                    font.bold: true
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 120
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Tiempo Final"
                    font.family: "Helvetica"
                    font.pointSize: 12
                    font.bold: true
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 120
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Diferencia"
                    font.family: "Helvetica"
                    font.pointSize: 12
                    font.bold: true
                }
            }
        }
    }

    Rectangle {
        id: baseList
        anchors.top: header.bottom
        width: root.width
        height: riderHeight*defRank
        color: "transparent"
        Rectangle {
            width: root.width
            height: riderHeight*topRiders
            color: "white"
            opacity: 0.8
        }

        Component {
            id: contactDelegate
            Item {
                width: root.width
                height: riderHeight

                RowLayout{
                    anchors.fill: parent
                    spacing: 0
                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: position
                            font.family: "Helvetica"
                            font.pointSize: 14
                            font.bold: true
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 180
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: {
                                switch (Qt.platform.os) {
                                case "linux": return -5
                                case "windows": return 0
                                }
                            }
                            Text {
                                Layout.leftMargin: 10
                                text: name
                                font.family: "Helvetica"
                                font.pointSize: 12
                            }
                            Text {
                                Layout.leftMargin: 10
                                text: club
                                font.family: "Helvetica"
                                font.pointSize: 10
                            }
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: riderHeight
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: categoria
                            font.family: "Helvetica"
                            font.pointSize: 12
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: number
                            font.family: "Helvetica"
                            font.pointSize: 14
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: chronoTime
                            font.family: "Helvetica"
                            font.pointSize: 12
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: diffTime
                            font.family: "Helvetica"
                            font.pointSize: 12
                        }
                    }
                }
            }
        }

        ListView {
            id: resultlist
            width: baseList.width;
            height: baseList.height
            model: RiderResultModel {id: riderResultModel}
            delegate: contactDelegate
//            highlight: Rectangle {
//                width: 490
//                height: 60
//                color: "lightsteelblue"
//                radius: 5
//            }
            focus: true
            clip:true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {active: true}
        }
    }
}
