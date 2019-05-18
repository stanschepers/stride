import QtQuick 2.5

Item {

    ListView {
        anchors.fill: parent
        anchors.margins: 20

        clip: true

        model: epiOutput

        delegate: healthStatusDelegate

        section.property: "ageBracket"
        section.delegate: ageBracketDelegate
    }

    Component {
        id: healthStatusDelegate

        Text {
            width: ListView.view.width
            height: 40
            text: healthStatus
//            fontColor: '#e0e0e0'
        }
    }

    Component {
        id: ageBracketDelegate

        Rectangle {
            width: ListView.view.width
            height: 40
            radius: 5
            color: "white"

            border.width: 1
            border.color: "#696969"

            Text {
                horizontalAlignment: Text.AlignHCenter

                text: "<b>" + section + "</b>"

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }


    ListModel {
        id: epiOutput

//        ListElement { healthStatus: "Abdul Ahad Mohmand"; ageBracket: "Afganistan"; }
//        ListElement { healthStatus: "Marcos Pontes"; ageBracket: "Brazil"; }
//        ListElement { healthStatus: "Alexandar Panayotov Alexandrov"; ageBracket: "Bulgaria"; }
//        ListElement { healthStatus: "Georgi Ivanov"; ageBracket: "Bulgaria"; }
//        ListElement { healthStatus: "Roberta Bondar"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Marc Garneau"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Chris Hadfield"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Guy Laliberte"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Steven MacLean"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Julie Payette"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Robert Thirsk"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Bjarni Tryggvason"; ageBracket: "Canada"; }
//        ListElement { healthStatus: "Dafydd Williams"; ageBracket: "Canada"; }
    }

    function addItem(health, age, value){
        epiOutput.append({ healthStatus: health + ": " + value, ageBracket: age });
    }

    function emptyList(){
        epiOutput.clear();
    }
}
