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
            ctrl.shownDay = value
            day.text = ctrl.shownDay
        }
    }

    Rectangle {
        id: dayRec
        color: "#ecf0f1"

        anchors.left: daySlider.right
        anchors.top: map.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Text {
            id: day
            text: ctrl.shownDay

            anchors.centerIn: dayRec
        }
    }

    Button {
        id: openDataButton
        text: "Show data"
        width: 200
        height: 50

        anchors.bottom: map.bottom
        anchors.right: map.right
        anchors.margins: 10

        onClicked: {
            dataBar.visible = true
        }
    }

    Rectangle {
        id: dataBar
        visible: false
        width: maxWidth(parent.width / 3, 400)
        radius: 10
        color: "#ecf0f1"

        border.width: 1
        border.color: "#696969"

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: map.bottom
        anchors.rightMargin: -radius

        Button {
            id: closeDataButton
            text: "Close"
            width: 100
            height: 50

            anchors.bottom: dataBar.bottom
            anchors.right: dataBar.right
            anchors.bottomMargin: 10
            anchors.rightMargin: 20

            onClicked: {
                dataBar.visible = false
            }
        }

        Text {
            id: dataBarlocationName
            wrapMode: Text.WordWrap
            text: ""

            anchors.horizontalCenter: dataBar.horizontalCenter
            anchors.top: dataBar.top
            anchors.right: root.right
            anchors.topMargin: 20
        }


        Text {
            id: dataBarEpiOutput
            text: ""

            anchors.top: dataBarlocationName.bottom
            anchors.right: root.right
            anchors.left: dataBar.left
            anchors.bottom: dataBar.bottom
            anchors.margins: 10
        }

    }



    function initialize(zoomlevel, centerLat, centerLong, firstDay, lastDay) {
        map.zoomLevel = zoomlevel;
        map.center = QtPositioning.coordinate(centerLat, centerLong);

        daySlider.minimumValue = firstDay;
        daySlider.maximumValue = lastDay;
        daySlider.value = firstDay;
    }

    function addLocation(locationId, latitude, longtitude, radius) {
        var component = Qt.createComponent("location.qml");
        if (component.status === Component.Ready) {
            var location = component.createObject(map);
            location.coorLat = latitude;
            location.coorLong = longtitude;
            location.rad = radius;
            location.col = Qt.rgba(Math.random(),Math.random(),Math.random(), 0.5);
            location.locationId = locationId;
            map.addMapItem(location);
        }
        var componentCentre = Qt.createComponent("location.qml");
        if (componentCentre.status === Component.Ready) {
            var centerCircle = componentCentre.createObject(map);
            centerCircle.coorLat = latitude;
            centerCircle.coorLong = longtitude;
            centerCircle.rad = radius / 100;
            centerCircle.col = "red";
            centerCircle.locationId = locationId;
            map.addMapItem(centerCircle);  // TODO: split this in seprate qml file
        }
        else
            console.log("Error loading component:", component.errorString());
    }

    function maxWidth(widthArg, maxWidthArg){
        if (widthArg > maxWidthArg){
            return maxWidthArg;
        }
        return widthArg;
    }

    function setData(locationName){
        dataBarlocationName.text = "<b>" + locationName + "</b>"
        // TODO: echte info inladen + miss naar een list overgaan (probeelm als je weg gaat van hover dan gaat de lijst weg dus een list is tsom, hoe oplsossen?)
        dataBarEpiOutput.text = 'AgeBracket:\n- Healthy: 25%\n- Infected: 48%\n- Recovered: 27%\n\nAgeBracket:\n- Healthy: 40%\n- Infected: 20%\n- Recovered: 40%\n\nAgeBracket:\n- Healthy: 40%\n- Infected: 20%\n- Recovered: 40%\n\nAgeBracket:\n- Healthy: 40%\n- Infected: 20%\n- Recovered: 40%'
    }

    function emptyData(){
        dataBarlocationName.text = ""
        dataBarEpiOutput.text = ""
    }

    onWidthChanged: ctrl.setWindowWidth = width
    onHeightChanged: ctrl.setWindowHeight = height

    Component.onCompleted: {
        ctrl.setWindowWidth = width
        ctrl.setWindowHeight = height
    }
}
