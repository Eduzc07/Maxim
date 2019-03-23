import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

Item{
    id: root
    property int riderHeight: 40
    property int defRank: 5
    property int numberRiders: {return count()>defRank?defRank:count()}

    width: 420 + riderHeight*3
    height: riderHeight + riderHeight*defRank + title.height

    function clearList(){
        riderModel.clear()
    }

    function count(){
        return riderModel.count
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
            text: "Lista de Partida"
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
            opacity: 0.8
        }
        RowLayout {
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
                    spacing: -5
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
                Layout.preferredWidth: 100
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Proc"
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
                Layout.preferredWidth: 100
                Layout.preferredHeight: riderHeight
                border.width: 0.5
                border.color: "black"

                Text {
                    anchors.centerIn: parent
                    text: "Inicio"
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
        width:  root.width;
        height: riderHeight*defRank
        color: "transparent"
        Rectangle {
            width:  root.width;
            height: riderHeight*numberRiders
            color: "white"
            opacity: 0.5
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
                            spacing: -5
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
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: home
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
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: riderHeight
                        border.width: 0.5
                        border.color: "black"

                        Text {
                            anchors.centerIn: parent
                            text: start
                            font.family: "Helvetica"
                            font.pointSize: 12
                        }
                    }
                }
            }
        }

        ListView {
            id: riderlist
            width: baseList.width;
            height: baseList.height
            model: RiderModel {id: riderModel}
            delegate: contactDelegate
            highlight: Rectangle {
                width: root.width
                height: riderHeight
                color: "lightsteelblue"
                radius: 5
            }
            focus: true
            clip:true
//            displaced: Transition {
//                NumberAnimation { properties: "x,y"; duration: 1000 }
//            }

            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {active: true}
        }
    }
}
