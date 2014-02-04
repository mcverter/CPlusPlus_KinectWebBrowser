
import QtQuick 1.0

Rectangle
{
    // Properties for all keys
    id: rect
    border.color : "grey"
    border.width : keyHeight/25
    color : "silver"
    radius : 5

    // keyWidth and key Height defined in VirtualKeyboard
    width: keyWidth * relative_width
    height: keyHeight

    // Properties acquired from MODEL
    property string key_type: ""
    property int relative_width: 1
    property int fontsize_ratio: 1
    property int dropDownSpeed: 1000

    // Key text
    Text
    {
        text: lowercase ? lower : upper;
        width: parent.width
        height: parent.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        property real base_font_size: Math.min(width,height) /3
	font.pointSize: (base_font_size / fontsize_ratio) > 0 ?
                            (base_font_size / fontsize_ratio) : 5

    }

    // Dropdown layover Box
    Rectangle
    {
        id : greyBox
        x: parent.border.width/2
        y: x
        width: parent.width - parent.border.width
        height: 0
        color: Qt.lighter("green")  //"lightsteelblue"
        opacity: 0.5
        radius: parent.radius
//        smooth: true
    }

    // Animating Dropdown Layover
    PropertyAnimation
    {
        id: dropdownLayover
        target: greyBox
        property: "height"
        to: keyHeight - greyBox.y
        duration: rect.dropDownSpeed
    }

    // Automatic Key Press timer
    Timer
    {
        id:  clickedTimer
          onTriggered: {keyPressed();}
        interval: rect.dropDownSpeed; running: false; repeat: false
    }

    // Mouse sensitivity
    MouseArea
    {
        id: mouseArea
        hoverEnabled:  true
        anchors.fill: parent
        onClicked: { keyPressed() }
      //  onEntered: { startTimedEvents() }
      //  onExited:  { stopTimedEvents() }
    }

    //FUNCTIONS

    // Timed Events start when mouse enters
    function startTimedEvents()
    {
        clickedTimer.start()
        dropdownLayover.start()
    }

    // Timed Events stop when mouse exits or key is pressed
    function stopTimedEvents()
    {
        clickedTimer.stop()
        dropdownLayover.stop()
        greyBox.height=0;
    }

    // A key press can trigger several diferent actions
    function keyPressed(key)
    {
        stopTimedEvents();

        if (key_type=='n')
        {
            normalKeySignal(lowercase ? lower : upper);
        }
        else if (key_type=='s')
        {
            changeCase()
        }
        else if (key_type=='b')
        {
            backspaceKeySignal()
        }
        else if (key_type=='e')
        {
            normalKeySignal("\n")
            enterKeySignal()
        }
        else
        {
            console.log("[KEYBOARD] Error in key identification.")
        }

    }

}
