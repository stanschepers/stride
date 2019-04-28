import QtQuick 2.0
import QtLocation 5.6
import QtQuick.Controls 2.1

MapCircle {
    property string idName
    property real coorLat
    property real coorLong
    property real rad
    property color col
    id: locationCircle
    center {
        latitude: coorLat
        longitude: coorLong
    }
    radius: rad
    color: col
    border.width: 1

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        hoverEnabled: true

//        onEntered: popup.open()
//        onExited: popup.close()
    }

    Popup {
            visible: mouseArea.containsMouse
            id: popup
            x: mouseArea.mouseX
            y: mouseArea.mouseY
            width: 200
            height: 300
            modal: true
            focus: true
            padding: 10

            contentItem: Text {
                text: locationCircle.idName
            }
        }

}
