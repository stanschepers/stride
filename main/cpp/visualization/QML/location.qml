import QtQuick 2.0
import QtLocation 5.6
import QtQuick.Controls 2.1

MapCircle {
    property real locationId
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
    border.color: "#696969"

    RoundMouseArea {
        id: mouseAreaCircle
        anchors.fill: parent

        onEntered: ctrl.setShownInformation = parent.locationId

        onClicked: { ctrl.dataPinned = !ctrl.dataPinned; mouseAreaCircle.visible = false; mouseAreaCircle.visible = true; }

        onExited: ctrl.setShownInformation = ""
    }

//    Popup {
//            focus: true
////            visible: /*locationCircle.showPopup &*/ (mouseAreaPopup.containsMouse | mouseAreaCircle.containsMouse)
//            id: popup
//            x: mouseAreaCircle.mouseX - 3.5 * width/4
//            y: mouseAreaCircle.mouseY - height/8
//            width: 200
//            height: 300

//            contentWidth: width
//            contentHeight: height
//            dim: false

//            enter: Transition {
//                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
//            }
//            exit: Transition {
//                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
//            }

//            background: Rectangle {
//                width: popup.width
//                height: popup.height
//                anchors.centerIn: parent
//                radius: 10
//                color: "#ecf0f1"
//                border.color: "#696969"
//                border.width: 1
//                Text {
//                    anchors.fill: parent
//                    anchors.margins: 3
//                    text: locationCircle.name
//                }

////                focus: true
////                Keys.onSpacePressed: {
////                    showPopup = !showPopup
////                    popup.focus = false
////                    focus = true
////                    print(showPopup)
////                }
//            }

//            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

//            MouseArea {
//                id: mouseAreaPopup
//                width: popup.width
//                height: popup.height
//                anchors.centerIn: parent
//                hoverEnabled: true

////                onExited: popup.close()
//            }
//        }

}
