import QtQuick 2.12
import QtQuick.Window 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11

Window {
    id: dataVisualWindow
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

        property bool disable_panning : false
        property int no_panning : (MapGestureArea.PinchGesture | MapGestureArea.FlickGesture |MapGestureArea.RotationGesture | MapGestureArea.TiltGesture)
        property int panning : (MapGestureArea.PanGesture | MapGestureArea.PinchGesture | MapGestureArea.FlickGesture |MapGestureArea.RotationGesture | MapGestureArea.TiltGesture)

        gesture.acceptedGestures: disable_panning ? no_panning : panning

        anchors.fill: parent
        anchors.bottomMargin: 20

        plugin: mapPlugin
        center: QtPositioning.coordinate(50.8503, 4.3517) // Brussels
        zoomLevel: 10

        MouseArea {
            id: mapSelectionMouseArea
            anchors.fill: parent

            acceptedButtons: Qt.RightButton

            property var selectedCoor;

            onPressed: {
                map.disable_panning = true;
                selectedCoor = map.toCoordinate(Qt.point(mouse.x,mouse.y));
                ctrl.dataPinned = false;
                dataVisualWindow.emptyData();

                if (selectionButton.text == "Rectangle"){
                    selectionRec.topLeft = map.toCoordinate(Qt.point(mouse.x,mouse.y));
                    selectionRec.bottomRight = map.toCoordinate(Qt.point(mouse.x,mouse.y));
                    selectionRec.visible = true;
                }
                else if (selectionButton.text == "Circle"){
                    selectionCircle.center = map.toCoordinate(Qt.point(mouse.x,mouse.y));
                    selectionCircle.radius = 0;
                    selectionCircle.visible = true;
                }
            }
            onPositionChanged: {
                if (map.disable_panning){
                    if (selectionButton.text == "Rectangle"){
                        selectionRec.bottomRight = map.toCoordinate(Qt.point(mouse.x,mouse.y));  // TODO: fix if corners switch that rectangle doesn't flip
                    }
                    else if (selectionButton.text == "Circle"){
                        selectionCircle.radius = selectedCoor.distanceTo(map.toCoordinate(Qt.point(mouse.x,mouse.y)));
                    }
                }
            }
            onReleased: {
                map.disable_panning = false;

                if (selectionButton.text == "Rectangle"){
                    ctrl.setShownInformation = selectedCoor.latitude + " " + selectedCoor.longitude + " " + map.toCoordinate(Qt.point(mouse.x,mouse.y)).latitude + " " + map.toCoordinate(Qt.point(mouse.x,mouse.y)).longitude;
                }
                else if (selectionButton.text == "Circle"){
                    ctrl.setShownInformation = selectedCoor.latitude + " " + selectedCoor.longitude + " " + selectionCircle.radius;
                }

                ctrl.dataPinned = true;

                selectionButton.text = "Clear"
            }
        }

        MapRectangle {
            id: selectionRec
            visible: false
            color: Qt.rgba(1, 1, 1, 0.3)

            border.color: "black"
            border.width: 2

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (!map.disable_panning){
                        console.log("No no no");
                        ctrl.dataPinned = false;
                    }
                }
            }
        }

        MapCircle {
            id: selectionCircle
            visible: false
            color: Qt.rgba(1, 1, 1, 0.3)

            border.color: "black"
            border.width: 2

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (!map.disable_panning){
                        console.log("No no no");
                        ctrl.dataPinned = false;
                    }
                }
            }
        }

        //focus: true

        //Keys.onSpacePressed: root.addLocation(50.8503, 4.3517, 3000)
        //Keys.onReturnPressed: map.clearMapItems()
    }

    Rectangle {
        id: coverUpCopyRight
        height: 60
        color: "#ecf0f1"

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Button {
        id: selectionButton
        text: "Rectangle"
        width: 160
        height: 40

        anchors.top: coverUpCopyRight.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 10

        onClicked: {
            if (selectionButton.text == "Rectangle"){
                selectionButton.text = "Circle";
            }
            else if (selectionButton.text == "Circle"){
                selectionButton.text = "Rectangle";
            }
            else if (selectionButton.text == "Clear"){
                selectionButton.text = "Rectangle";
                selectionCircle.visible = false;
                selectionRec.visible = false;
                ctrl.dataPinned = false;
            }
        }
    }

    Slider {
        id: daySlider;
        height: 40
        z: map.z + 3

        anchors.margins: 10
        anchors.rightMargin: parent.width * 0.1
        anchors.bottom: parent.bottom
        anchors.left: selectionButton.right
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
        anchors.top: coverUpCopyRight.top
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
        width: 160
        height: 40

        anchors.top: parent.top
        anchors.right: parent.right
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
        anchors.bottom: coverUpCopyRight.top
        anchors.rightMargin: -radius

        Text {
            id: pinnedText
            text: "Pinned (click to unpin)"
            font.pointSize: 6
            visible: false

            anchors.top: dataBar.top
            anchors.left: dataBar.left
            anchors.right: closeDataButton.left
            anchors.margins: 10

            MouseArea {
                anchors.fill: parent
                onClicked: { ctrl.dataPinned = !ctrl.dataPinned; dataVisualWindow.emptyData(); }
            }

        }

        Button {
            id: closeDataButton
            text: "X"
            width: 30
            height: 30

            anchors.top: dataBar.top
            anchors.right: dataBar.right
            anchors.topMargin: 10
            anchors.rightMargin: 20

            onClicked: {
                dataBar.visible = false
            }
        }

        Text {
            id: dataBarlocationName
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            text: ""

            anchors.horizontalCenter: dataBar.horizontalCenter
            anchors.top: closeDataButton.bottom
            anchors.left: dataBar.left
            anchors.topMargin: 5
            anchors.leftMargin: 5
            anchors.rightMargin: 15
        }

        Rectangle {
            id: seperation
            height: 1
            color: "#696969"

            anchors.top: dataBarlocationName.bottom
            anchors.right: dataBar.right
            anchors.left: dataBar.left
        }

        DataBarList {
            id: dataBarEpiOutput

            anchors.top: seperation.bottom
            anchors.right: dataBar.right
            anchors.left: dataBar.left
            anchors.bottom: dataBar.bottom
        }

        focus: true
        Keys.onEscapePressed: visible = false

    }

    ComboBox {
        id: ageBracketComboBox
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        width: 240
        model: ["Age bracket", "Daycare", "PreSchool", "K12School", "College", "Workplace", "Senior"]

        onActivated: ctrl.setAgeBracket = model[index]
    }

    ComboBox {
        id: healthStatusComboBox
        anchors.top: parent.top
        anchors.left: ageBracketComboBox.right
        anchors.margins: 10
        width: 240
        model: ["Health status", "Susceptible", "Infected", "Infectious", "Symptomatic", "Recovered", "Immune"]

        onActivated: ctrl.setHealthStatus = model[index]
    }

    Rectangle {
        id: colorSpectrum
        width: 30
        radius: 5
        visible: false;

        border.width: 1
        border.color: "#696969"

        anchors.left: parent.left
        anchors.top: ageBracketComboBox.bottom
        anchors.bottom: coverUpCopyRight.top
        anchors.bottomMargin: 20
        anchors.topMargin: 20
        anchors.leftMargin: 10

        gradient: Gradient {
            GradientStop {
                position: 0.0; color: Qt.hsva(0.7, 1, 1, 0.7)
            }
            GradientStop {
                position: 0.5; color: Qt.hsva(0, 1, 1, 0.7)
            }
            GradientStop {
                position: 1.0; color: Qt.hsva(0.3, 1, 1, 0.7)
            }
        }

        Rectangle {
            id: spectrumHighRec
            width: 160
            height: 40
            radius: 10
            color: "#ecf0f1"

            border.width: 1
            border.color: "#696969"

            anchors.left: parent.right
            anchors.top: parent.top

            Text {
                id: spectrumHighValue
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("100%")

                anchors.fill: parent
                anchors.margins: 5
            }
        }

        Rectangle {
            id: spectrumLowRec
            width: 160
            height: 40
            radius: 10
            color: "#ecf0f1"

            border.width: 1
            border.color: "#696969"

            anchors.left: parent.right
            anchors.bottom: parent.bottom

            Text {
                id: spectrumLowValue
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("0%")

                anchors.fill: parent
                anchors.margins: 5
            }
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
            location.col = Qt.hsva(Math.random(), 1, 1, 0.5);
//            location.col = Qt.rgba(Math.random(),Math.random(),Math.random(), 0.5);
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
            map.addMapItem(centerCircle);
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

    function setData(locationName, epiOutput){
        dataBarlocationName.text = "<b>" + locationName + "</b>";
        var ageBrackets = ["Daycare", "PreSchool", "K12School", "College", "Workplace", "Senior"]
        var healthStatuses = ["Total", "Susceptible", "Infected", "Infectious", "Symptomatic", "Recovered", "Immune"]
        for (var i = 0; i < ageBrackets.length; i++){
            for (var j = 0; j < healthStatuses.length; j++){
                dataBarEpiOutput.addItem(healthStatuses[j], ageBrackets[i], epiOutput[ageBrackets[i]][healthStatuses[j]]);
            }
        }
    }

    function emptyData(){
        dataBarlocationName.text = "";
        dataBarEpiOutput.emptyList();
    }

    function updateLocation(locationId, value){
        var children = map.children;
        for (var i = 0; i < children.length; ++i){
            if (children[i].locationId === locationId){
                if (value === -1){
                    children[i].color = Qt.hsva(Math.random(), 1, 1, 0.5);
                } else {
                    var high = 500;
                    var low = 0.1;
                    value = -0.6 * value + 0.3;
//                    value = -0.6 * (Math.log((high - low) * value + low) - Math.log(low))/(Math.log(high) - Math.log(low)) + 0.3;
                    if (value < 0){
                        value = 1 + value;
                    }
                    children[i].color = Qt.hsva(value, 1, 1, 0.5);
                }
            }
        }
    }

    function updateColorSpectrumValues(low, high){
        if (low == -1 && high == -1){
            colorSpectrum.visible = false;
        } else {
            colorSpectrum.visible = true;
            spectrumLowValue.text = low;
            spectrumHighValue.text = high;
        }
    }

    function togglePinned(value){
        pinnedText.visible = value;
        if (value == false){
            selectionCircle.visible = false;
            selectionRec.visible = false;
            dataVisualWindow.emptyData();
            if (selectionButton.text == "Clear"){
                selectionButton.text = "Rectangle";
            }
        }
    }

    onWidthChanged: ctrl.setWindowWidth = width
    onHeightChanged: ctrl.setWindowHeight = height

    Component.onCompleted: {
        ctrl.setWindowWidth = width
        ctrl.setWindowHeight = height
    }
}
