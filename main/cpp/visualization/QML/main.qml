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

        Keys.onSpacePressed: root.addLocation()
        Keys.onReturnPressed: map.clearMapItems()
    }

    function addLocation(){
        var component = Qt.createComponent("location.qml");
        if (component.status == Component.Ready) {
            var location = component.createObject(map);
            location.coorLat = 50.8503
            location.coorLong = 4.3517
            location.rad = 5000
            location.col = '#800000FF'
            map.addMapItem(location)
        }
        else
            console.log("Error loading component:", component.errorString());
    }
}
