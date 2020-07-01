import QtQuick 2.0
import QtQuick.Controls 2.2
import Recorder 1.0

ApplicationWindow {
    id: mainWndow
    width: 400
    height: 500
    visible: true


    Recorder {
        id: recorder
    }

    Rectangle {
        id: magicButtonRectnagle
        anchors.centerIn: parent
        width: 100
        height: 50
        color: "green"
        Text {
            id: magicButtonText
            text: qsTr("Запись")
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: recorder.startRecord()

        }
    }

    Rectangle {
        id: devicesList
        anchors.bottom: magicButtonRectnagle.top
        anchors.bottomMargin: 10
        height: 200
        width: 400
        Text {
            id: devicesLabel
            text: qsTr("Devices")
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
        ListView {
            anchors.fill: parent
            model: recorder.devices
            delegate: Rectangle {
                height: 25
                width: 100
                Text { text: display }
            }
        }
    }
}
