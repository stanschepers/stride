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
    color: "#ecf0f1"


    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
    }

    Map {
        id: map
        anchors.fill: parent
        anchors.bottomMargin: 60
        plugin: mapPlugin
        center: QtPositioning.coordinate(50.8503, 4.3517) // Brussels
        zoomLevel: 10
        //focus: true

        //Keys.onSpacePressed: root.addLocation(50.8503, 4.3517, 3000)
        //Keys.onReturnPressed: map.clearMapItems()
    }

    function addLocation(locationName, latitude, longtitude, radius) {
        var component = Qt.createComponent("location.qml");
        if (component.status === Component.Ready) {
            var location = component.createObject(map);
            location.coorLat = latitude;
            location.coorLong = longtitude;
            location.rad = radius;
            location.col = Qt.rgba(Math.random(),Math.random(),Math.random(), 0.5);
            location.name = locationName;
            map.addMapItem(location);
        }
        var componentCentre = Qt.createComponent("location.qml");
        if (componentCentre.status === Component.Ready) {
            var centerCircle = componentCentre.createObject(map);
            centerCircle.coorLat = latitude;
            centerCircle.coorLong = longtitude;
            centerCircle.rad = radius / 100;
            centerCircle.col = "red";
            centerCircle.name = "center" + locationName;
            map.addMapItem(centerCircle);  // TODO: split this in seprate qml file
        }
        else
            console.log("Error loading component:", component.errorString());
    }

    Slider {
        id: daySlider;
        z: map.z + 3
        anchors.margins: 10
        anchors.rightMargin: parent.width * 0.1
        anchors.top: map.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        orientation : Qt.Horizontal
        onValueChanged: {
            ctrl.setDay = value
            day.text = ctrl.getDay
        }
    }

    Rectangle {
        id: rec1
        anchors.left: daySlider.right
        anchors.top: map.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        color: "#ecf0f1"

        Text {
            id: day
            text: ctrl.getDay

            anchors.centerIn: rec1
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

    onWidthChanged: ctrl.setWindowWidth = width
    onHeightChanged: ctrl.setWindowHeight = height

    Component.onCompleted: {
        ctrl.setWindowWidth = width
        ctrl.setWindowHeight = height
    }
}
