import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3

Item {
    id: root
    property alias startTime: startTimeField.text
    property alias timeRider: timeRiderField.text
    property alias timeCat: timeCatField.text

    property alias enableB1: button1.enabled
    property alias enableB2: button2.enabled

    property bool bSave: false
    property string nameButton1: "Load"
    property string nameButton2: "Save"

    opacity: root.enabled?1:0.8

    signal clickButton1()
    signal clickButton2()
    signal signalMessageOk()

    function showMessage(){
        messageDialog.visible = true
    }

    function fileOpen(){
        fileDialog.open()
    }

    MessageDialog {
        id: messageDialog
        title: "Advertencia!!!!"
        text: "Error en hora de Inicio:"
        informativeText: "La lista debe generarse al menos 10 min antes de iniciar la carrera."
        icon: StandardIcon.Warning
        onAccepted: signalMessageOk()
    }

    FileDialog {
        id: fileDialog
        title: "Guardar"
        selectExisting: false
        nameFilters: [ "CSV (*.csv)" ]
        onAccepted: {
            if (topLeftArea.bSave){
                readdata.saveFile(fileDialog.fileUrls)
                topLeftArea.bSave = false
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.8
        }

        Row {
            spacing: 20
            anchors.centerIn: parent
            Column{
                spacing: 10
                Text {
                    text: "Hora de Inicio: "
                    font.family: "Helvetica"
                    font.pointSize: 14
                    color: "white"
                    horizontalAlignment: Text.AlignRight
                }

                Text {
                    text: "Intervalo entre Corredores (seg): "
                    font.family: "Helvetica"
                    font.pointSize: 14
                    color: "white"
                    horizontalAlignment: Text.AlignRight
                }
                Text {
                    text: "Intervalo entre Categorias (seg): "
                    font.family: "Helvetica"
                    font.pointSize: 14
                    color: "white"
                    horizontalAlignment: Text.AlignRight
                }
            }

            Column{
                spacing: 10
                TextField {
                    id: startTimeField
                    width: 90
                    height: 30
                    font.pointSize: 12
                    color: "white"
                    text: qsTr("00:00:00")
                    placeholderText: qsTr("hh:mm:ss")
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    background: Rectangle {
                                radius: 2
                                implicitWidth: 100
                                implicitHeight: 24
                                color: "black"
                                opacity: 0.5
                                border.color: "white"
                                border.width: 1
                            }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                    }
                }
                TextField {
                    id: timeRiderField
                    width: 90
                    height: 30
                    font.pointSize: 12
                    color: "white"
                    text: qsTr("60")
                    placeholderText: qsTr("60")
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    background: Rectangle {
                                radius: 2
                                implicitWidth: 100
                                implicitHeight: 24
                                color: "black"
                                opacity: 0.5
                                border.color: "white"
                                border.width: 1
                            }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                    }
                }
                TextField {
                    id: timeCatField
                    width: 90
                    height: 30
                    font.pointSize: 12
                    color: "white"
                    text: qsTr("120")
                    placeholderText: qsTr("120")
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    background: Rectangle {
                                radius: 2
                                implicitWidth: 100
                                implicitHeight: 24
                                color: "black"
                                opacity: 0.5
                                border.color: "white"
                                border.width: 1
                            }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                    }
                }
            }

            Column{
                spacing: 10
                anchors.verticalCenter: parent.verticalCenter
                Button {
                    id: button1
                    width: 140
                    height: 40
                    text: nameButton1
                    onClicked: clickButton1 ()
                }
                Button {
                    id: button2
                    width: 140
                    height: 40
                    text: nameButton2
                    onClicked: clickButton2()
                }
            }
        }
    }
}

