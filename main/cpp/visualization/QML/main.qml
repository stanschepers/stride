import QtQuick 2.12
import QtQuick.Window 2.0
import QtLocation 5.6
import QtPositioning 5.6

Window {
    id: root
    width: 512
    height: 512
    visible: true

    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(50.8503, 4.3517) // Brussels
        zoomLevel: 10
        focus: true

//        Keys.onSpacePressed: root.addLocation()
//        Keys.onReturnPressed: map.clearMapItems()
    }

//    Connections {
//        target: MapController
//        onDoAddLocation: {
//            addLocation(latitude, longtitude, radius)
//        }
//    }

    function addLocation(latitude, longtitude, radius){
        var component = Qt.createComponent("location.qml");
        if (component.status == Component.Ready) {
            var location = component.createObject(map);
            location.coorLat = latitude
            location.coorLong = longtitude
            location.rad = radius
            location.col = Qt.rgba(Math.random(),Math.random(),Math.random(), 0.5);
            map.addMapItem(location)
        }
        else
            console.log("Error loading component:", component.errorString());
    }
}
