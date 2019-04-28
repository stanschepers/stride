import QtQuick 2.12
import QtQuick.Window 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 1.4

Window {
    id: root
    width: 1024
    height: 1024
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
        //focus: true

        //Keys.onSpacePressed: root.addLocation(50.8503, 4.3517, 3000)
        //Keys.onReturnPressed: map.clearMapItems()
    }

    function addLocation(locationId, latitude, longtitude, radius) {
        var component = Qt.createComponent("location.qml");
        if (component.status == Component.Ready) {
            var location = component.createObject(map);
            location.coorLat = latitude;
            location.coorLong = longtitude;
            location.rad = radius;
            location.col = Qt.rgba(Math.random(),Math.random(),Math.random(), 0.5);
            location.idName = locationId;
            map.addMapItem(location);
        }
        var componentCentre = Qt.createComponent("location.qml");
        if (componentCentre.status == Component.Ready) {
            var centerCircle = componentCentre.createObject(map);
            centerCircle.coorLat = latitude;
            centerCircle.coorLong = longtitude;
            centerCircle.rad = radius / 100;
            centerCircle.col = "red";
            centerCircle.idName = "center" + locationId;
            map.addMapItem(centerCircle);
        }
        else
            console.log("Error loading component:", component.errorString());
    }

    Slider {
        id: daySlider;
        z: map.z + 3
        anchors.margins: 10
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        orientation : Qt.Horizontal
        onValueChanged: {
            ctrl.setDay = value
        }
    }

    function initialize(zoomlevel, centerLat, centerLong, firstDay, lastDay) {
//        console.log(zoomlevel)
//        console.log(centerLat)
//        console.log(centerLong)
//        console.log(firstDay)
//        console.log(lastDay)
        map.zoomLevel = zoomlevel;
        map.center = QtPositioning.coordinate(centerLat, centerLong);

        daySlider.minimumValue = firstDay;
        daySlider.maximumValue = lastDay;
        daySlider.value = firstDay;
    }

    function getWindowSize(){
        ctrl.setWindowWidth = width
        ctrl.setWindowHeight = height
    }

    onWidthChanged: ctrl.setWindowWidth = width
    onHeightChanged: ctrl.setWindowHeight = height
}
