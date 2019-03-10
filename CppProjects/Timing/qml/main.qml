import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import io.qt.examples.backend 1.0

Window {
    id: root
    width: 600
    height: 480
    visible: true

    BackEnd {
        id: backend
    }

    TextField {
        text: backend.userName
        placeholderText: qsTr("User name")
        anchors.centerIn: parent

        onTextChanged: backend.userName = text
    }
}
