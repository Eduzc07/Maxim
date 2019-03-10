import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

Item{
    id: root
    width: 520;
    height: 60 + 60*9

    function clearList(){
        riderModel.clear()
    }

    function addList(newRider){
        riderModel.append(newRider)
    }

    function getStartTime(){
        return riderModel.get(0).start
    }

    function getNextStartTime(idx){
        return riderModel.get(idx).start
    }

    function getNumber(){
        return riderModel.get(0).number
    }

    function getRiderInfo(){
        return riderModel.get(0)
    }

    function getPosition(){
        var pos = riderModel.get(0).position
        return Number.fromLocaleString(pos)
    }

    function remove(idx){
        riderModel.remove(idx)
    }

    Rectangle {
        id: header
        width: 520
        height: 60
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: 0.8
        }
        RowLayout{
            anchors.fill: parent
            spacing: 0
            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 60
                Layout.preferredHeight: 60
                border.width: 1
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Pos"
                    font.family: "Helvetica"
                    font.pointSize: 16
                    font.bold: true
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 250
                Layout.preferredHeight: 60
                border.width: 1
                border.color: "black"

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: -5
                    Text {
                        Layout.leftMargin: 10
                        text: "Nombre"
                        font.family: "Helvetica"
                        font.pointSize: 18
                    }
                    Text {
                        Layout.leftMargin: 10
                        text: "Club"
                        font.family: "Helvetica"
                        font.pointSize: 12
                    }
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 90
                Layout.preferredHeight: 60
                border.width: 1
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "#"
                    font.family: "Helvetica"
                    font.pointSize: 20
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 120
                Layout.preferredHeight: 60
                border.width: 1
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Inicio"
                    font.family: "Helvetica"
                    font.pointSize: 14
                    font.bold: true
                }
            }
        }
    }

    Rectangle {
        anchors.top: header.bottom
        width: 520;
        height: 60*9
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: 0.5
        }

        Component {
            id: contactDelegate
            Item {
                width: root.width
                height: 60

                RowLayout{
                    anchors.fill: parent
                    spacing: 0
                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: 60
                        border.width: 1
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: position
                            font.family: "Helvetica"
                            font.pointSize: 24
                            font.bold: true
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 250
                        Layout.preferredHeight: 60
                        border.width: 1
                        border.color: "black"

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: -5
                            Text {
                                Layout.leftMargin: 10
                                text: name
                                font.family: "Helvetica"
                                font.pointSize: 18
                            }
                            Text {
                                Layout.leftMargin: 10
                                text: club
                                font.family: "Helvetica"
                                font.pointSize: 12
                            }
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 90
                        Layout.preferredHeight: 60
                        border.width: 1
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: {
                                if (number === ""){
                                    ""
                                } else {
                                    '<b>#</b> ' + number}
                            }
                            font.family: "Helvetica"
                            font.pointSize: 20
                        }
                    }

                    Rectangle {
                        color: 'transparent'
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 60
                        border.width: 1
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: start
                            font.family: "Helvetica"
                            font.pointSize: 14
                        }
                    }
                }
            }
        }

        ListView {
            id: riderlist
            anchors.fill: parent
            model: RiderModel {id: riderModel}
            delegate: contactDelegate
            highlight: Rectangle {
                width: 490
                height: 60
                color: "lightsteelblue"
                radius: 5
            }
            focus: true
        }
    }
}
