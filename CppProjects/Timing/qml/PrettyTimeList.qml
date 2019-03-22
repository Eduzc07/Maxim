import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

Item {
    id: root
    width: 480
    height: 400

    ListModel {
        id: viewModel
        ListElement {
            position: "1"
            name: "Bill Smith"
            categoria: "D"
            club: "saqras"
            home: "Cusco"
            number: "16"
            startTime: "00:00:00"
            endTime: "00:00:00"
            chronoTime: "00:00.000"
            diffTime: "+00:00.000"
        }
    }
}
