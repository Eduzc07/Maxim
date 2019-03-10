import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

Item{
    id: root
    width: 540;
    height: 60 + 60*count()

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
        var pos = Number(position).toLocaleString()
        riderResultModel.setProperty(idx, "position", pos)
    }

    function setElapsed(idx, elapsed){
        riderResultModel.setProperty(idx, "diffTime", elapsed)
    }

    function getInfo(idx){
        return riderResultModel.get(idx)
    }

    Rectangle {
        id: header
        width: root.width
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
                    font.pointSize: 14
                    font.bold: true
                }
            }

            Rectangle {
                color: 'transparent'
                Layout.preferredWidth: 150
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
                        font.pointSize: 14
                    }
                    Text {
                        Layout.leftMargin: 10
                        text: "club"
                        font.family: "Helvetica"
                        font.pointSize: 10
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
                    text: "Llegada"
                    font.family: "Helvetica"
                    font.pointSize: 14
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
                    text: "Diff"
                    font.family: "Helvetica"
                    font.pointSize: 14
                }
            }
        }
    }

    Rectangle {
        anchors.top: header.bottom
        width: root.width
        height: 60*count()
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
                        Layout.preferredWidth: 150
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
                                font.pointSize: 14
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
                            text: chronoTime
                            font.family: "Helvetica"
                            font.pointSize: 14
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
                            text: diffTime
                            font.family: "Helvetica"
                            font.pointSize: 14
                        }
                    }
                }
            }
        }

        ListView {
            id: resultlist
            anchors.fill: parent
            model: RiderResultModel {id: riderResultModel}
            delegate: contactDelegate
//            highlight: Rectangle {
//                width: 490
//                height: 60
//                color: "lightsteelblue"
//                radius: 5
//            }
            focus: true
        }
    }
}
