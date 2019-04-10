import QtQuick 2.0
import QtLocation 5.6

MapCircle {
    property var idName;
    property real coorLat;
    property real coorLong;
    property real rad;
    property color col;
    id: idName
    center {
        latitude: coorLat
        longitude: coorLong
    }
    radius: rad
    color: col
    border.width: 1
}
