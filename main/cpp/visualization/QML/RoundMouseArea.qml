import QtQuick 2.0

Item {
    id: roundMouseArea

    property alias mouseX: mouseArea.mouseX
    property alias mouseY: mouseArea.mouseY

    property bool containsMouse: {
        if(!mouseArea.containsMouse)
            return false;

        var x1 = width / 2;
        var y1 = height / 2;
        var x2 = mouseX;
        var y2 = mouseY;
        var distanceFromCenter = Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2);
        var radiusSquared = Math.pow(Math.min(width, height) / 2, 2);
        var isWithinOurRadius = distanceFromCenter < radiusSquared;
        return isWithinOurRadius;
    }

    onContainsMouseChanged: {
        if (containsMouse === true){
            roundMouseArea.entered()
        } else {
            roundMouseArea.exited()
        }
    }

    signal clicked

    signal entered

    signal exited

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: if (roundMouseArea.containsMouse) roundMouseArea.clicked()
    }
}
