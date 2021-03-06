import QtQuick 2.0;

Item {

    property string position                   : "left";
    property string listBackgroundColor        : "white"
    property string handleColorNormal          : "black"
    property string handleColorPressed         : "blue"

    id: scrollbar;
    width: (handleSize + 2 * (backScrollbar.border.width +1));
//    visible: (flickable.visibleArea.heightRatio < 1.0);
    visible: (flickable.height >= parent.height);

    anchors {
        top: flickable.top;
        left: (position === "left") ? flickable.parent.left : undefined;
        right: (position === "right") ? flickable.parent.right : undefined;
        bottom: flickable.bottom;
        margins: 0;
    }

    signal barClicked();
    signal barReleased();

    property Flickable flickable               : null;
    property int       handleSize              : 9 * scaleX;

    //Proprietà che espone il timer che fa scomparire la barra dopo il termine dell'input u
    property alias barOpacity: backHandle.opacity

    //Valore di defualt dell'opacità per la ScrollBar
    property real defaultOpacity: 0.55

    function scrollDown () {
        flickable.contentY = Math.min (flickable.contentY + (flickable.height / 4), flickable.contentHeight - flickable.height);
    }
    function scrollUp () {
        flickable.contentY = Math.max (flickable.contentY - (flickable.height / 4), 0);
    }




   Binding {
        target: handle;
        property: "y";
        value: (flickable.contentY * clicker.drag.maximumY / (flickable.contentHeight - flickable.height));
        when: (!clicker.drag.active);
    }
    Binding {
        target: flickable;
        property: "contentY";
        value: (handle.y * (flickable.contentHeight - flickable.height) / clicker.drag.maximumY);
        when: (clicker.drag.active || clicker.pressed);
    }
    Rectangle {
        id: backScrollbar;
        color: listBackgroundColor
//        radius: 2;
//        antialiasing: true;
//        color: "#FAF2F2";
//        border {
//            width: 0.5;
//            color: "#FAF2F2";
//        }
        anchors { fill: parent; }

        MouseArea {
            anchors.fill: parent;
            onClicked: { }
        }
    }
//    MouseArea {
//        id: btnUp;
//        height: width;
//        anchors {
//            top: parent.top;
//            left: parent.left;
//            right: parent.right;
//            margins: (backScrollbar.border.width +1);
//        }
//        onClicked: { scrollUp (); }

//        Text {
//            text: "<";
//            antialiasing: true
//            color: (btnUp.pressed ? "black" : "gray");
//            rotation: 90;
//            anchors.centerIn: parent;
//        }
//    }
//    MouseArea {
//        id: btnDown;
//        height: width;
//        anchors {
//            left: parent.left;
//            right: parent.right;
//            bottom: parent.bottom;
//            margins: (backScrollbar.border.width +1);
//        }
//        onClicked: { scrollDown (); }

//        Text {
//            antialiasing: true
//            text: "<";
//            rotation: -90
//            color: (btnDown.pressed ? "black" : "gray");
//            anchors.centerIn: parent;
//        }
//    }
    Item {
        id: groove;
        clip: true;
        anchors {
            fill: parent;
//            topMargin: (backScrollbar.border.width +1 + btnUp.height +1);
            topMargin: (backScrollbar.border.width +1);
            leftMargin: (backScrollbar.border.width +1);
            rightMargin: (backScrollbar.border.width +1);
//            bottomMargin: (backScrollbar.border.width +1 + btnDown.height +1);
            bottomMargin: (backScrollbar.border.width +1 +1);
        }

        MouseArea {
            id: clicker;
            drag {
                target: handle;
                minimumY: 0;
                maximumY: (groove.height - handle.height);
                axis: Drag.YAxis;
            }
            anchors { fill: parent; }
            onClicked: { flickable.contentY = (mouse.y / groove.height * (flickable.contentHeight - flickable.height)); }
            onPressed: barClicked()
            onReleased: barReleased()
        }

        Item {
            id: handle;
            height: Math.max (20, (flickable.visibleArea.heightRatio * groove.height));
            anchors {
                left: parent.left;
                right: parent.right;
            }

            Rectangle {
                id: backHandle;
                radius: 20
                antialiasing: true
                color: (clicker.pressed ? handleColorPressed : handleColorNormal);
//                opacity: (flickable.moving ? 0.65 : 0.35);
                opacity: defaultOpacity;
                anchors.fill: parent

                Behavior on opacity { NumberAnimation { duration: 200; } }
            }
        }
    }
}
