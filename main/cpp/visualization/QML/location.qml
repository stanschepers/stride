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

//    MouseArea {
//        id: mouseArea
//        anchors.fill: parent

//        hoverEnabled: true

//        onEntered: popup.open()
//    }

    RoundMouseArea {
        id: mouseArea
        anchors.fill: parent

//        onEntered: popup.open()
    }

    Popup {

            visible: mouseAreaPopup.containsMouse || mouseArea.containsMouse
            id: popup

            x: /*parent.x*/ mouseArea.mouseX - 3.5 * width/4
            y: /*parent.y*/ mouseArea.mouseY - height/8
            width: 200
            height: 300
            modal: true
            focus: true

            enter: Transition {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
            }
            exit: Transition {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
            }

            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            contentItem: Text {
                text: locationCircle.idName
            }

            MouseArea {
                id: mouseAreaPopup
                anchors.fill: parent

                hoverEnabled: true

//                onExited: popup.close()
            }
        }

}
