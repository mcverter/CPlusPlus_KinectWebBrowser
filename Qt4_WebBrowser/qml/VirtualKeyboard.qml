// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Rectangle {
    id: root
    width: parent.width
    height: parent.height

    property int keyWidth : root.width/10
    property int keyHeight : root.height/6
    property bool lowercase : true
    property string target

    // SIGNALS
    signal normalKeySignal(string key)
    signal backspaceKeySignal()
    signal enterKeySignal()

    //FUNCTIONS
    // Called by shift keypress
    function changeCase()
    {
        if (lowercase==true)
            lowercase=false
        else
            lowercase=true
    }

    // Called by enter Keypress
    function hideKeyboard()
    {
        root.state = "hidden"
    }

    function showKeyboard()
    {
        root.state = "shown"
    }

    function changeTextDisp(text)
    {
        textDisp.text = text;
    }


    // STATES for hiding and showing keyboard
    states: [
        State
        {
            name: "hidden"

            AnchorChanges
            {
                target: root
                anchors.top : parent.bottom
            }
        },
        State
        {
            name: "shown"

            AnchorChanges
            {
                target: root
                anchors.top : parent.top
            }
        }
    ]


    // TRANSITION for hiding and showing keyboard
    transitions: Transition {
        AnchorAnimation { duration: 500 }
        PropertyAnimation {duration: 500 }
    }


    // To interface with VirtualKeyboardKey
    Component
    {
        id : letterDelegate
        VirtualKeyboardKey
        {
            fontsize_ratio: fzr
            key_type: kt
            relative_width: rw
        }
    }


    // First Two Rows of Keys

    Item
    {
        id: firstTwoRows
        width: keyWidth*10
        height: keyHeight*2
        anchors
        {
            top: root.top
            right: root.right
            left: root.left
        }

        // Normal Keys
        ListModel
        {
            id:  firstKeys
            ListElement { lower: "1"; upper: "!" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "2"; upper: "@" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "3"; upper: "#"  ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "4"; upper: "$" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "5"; upper: "%" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "6"; upper: "^" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "7"; upper: "&" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "8"; upper: "*" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "9"; upper: "(" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "0"; upper: ")" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "q"; upper: "Q" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "w"; upper: "W" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "e"; upper: "E" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "r"; upper: "R" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "t"; upper: "T" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "y"; upper: "Y" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "u"; upper: "U" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "i"; upper: "I" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "o"; upper: "O" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "p"; upper: "P";  fzr: 1; kt: "n"; rw: 1}
        }

        // Layout keys
        GridView
        {

            width: keyWidth*10; height: keyHeight*2
            cellWidth: keyWidth; cellHeight: keyHeight
            model: firstKeys
            delegate: letterDelegate
            interactive: false
        }
    }  // END First two rows of Keys


    // Third Row of Keys
    Item
    {
        id: thirdRow
        width: keyWidth*9
        height: keyHeight
        anchors.top: firstTwoRows.bottom

        // Normal Keys
        ListModel
        {
            id:  thirdKeys
            ListElement {lower: "a"; upper: "A"; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower:  "s"; upper: "S" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "d"; upper: "D" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower:  "f"; upper: "F" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "g"; upper: "G" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "h"; upper: "H" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "j"; upper: "J" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "k"; upper: "K" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "l"; upper: "L" ; fzr: 1; kt: "n"; rw: 1}
        }

        // Layout Normal Keys
        GridView
        {
            id: thirdLine
            width: keyWidth*9; height: keyHeight
            cellWidth: keyWidth; cellHeight: keyHeight
            model: thirdKeys
            delegate: letterDelegate
            interactive: false
        }

        // Backspace Key -- kt (keytype) is 'b'; fzr (fontsizeratio): 1.5
        ListModel
        {
            id: backList
            ListElement {lower: "Back\nspace"; upper: "Back\nspace" ; fzr: 1.5; kt: "b"; rw: 1}

        }
        // Layout Backspace Key
        GridView
        {
            id:  backGrid
            model: backList
            delegate:  letterDelegate
            height: keyHeight
            width: keyWidth
            interactive: false


            anchors
            {
                top: thirdRow.top
                bottom: thirdRow.bottom
                left: thirdRow.right
            }

        }

    }  // End Third Row of Keys


    // Fourth Row of Keys
    Item
    {
        id: fourthRow
        width: keyWidth*9
        height: keyHeight
        anchors.top: thirdRow.bottom


        // Shift Key -- kt (keytype) is 's', fzr (fontsizeratio): 1.5
        ListModel
        {
            id: shiftList
            ListElement {lower: "SYM"; upper: "sym" ; fzr: 1.5; kt: "s"; rw: 1}
        }
        // Layout Shift Key
        GridView
        {
            id:  shiftGrid
            model: shiftList
            delegate:  letterDelegate
            height: keyHeight
            width: keyWidth
            anchors
            {
                top: fourthRow.top
                bottom: fourthRow.bottom
                left: fourthRow.left
            }
        }

        // Normal Keys
        ListModel
        {
            id:  fourthKeys
            ListElement {lower: "z"; upper: "Z" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "x"; upper: "X" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "c"; upper: "C" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "v"; upper: "V" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "b"; upper: "B" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "n"; upper: "N" ; fzr: 1; kt: "n"; rw: 1}
            ListElement {lower: "m"; upper: "M" ; fzr: 1; kt: "n"; rw: 1}
        }

        // Layout Normal Keys
        GridView
        {
            id: fourthLine
            anchors
            {
                top: fourthRow.top
                bottom: fourthRow.bottom
                left: shiftGrid.right
            }
            width: keyWidth*7; height: keyHeight
            cellWidth: keyWidth; cellHeight: keyHeight
            model: fourthKeys
            delegate: letterDelegate
            interactive: false
        }

        // Enter Key width = 2
        ListModel
        {
            id: enterList
            ListElement {lower: "Enter"; upper: "Enter" ; fzr: 1.5; kt: "e"; rw: 2}
        }

        // Layout Enter Key
        GridView
        {
            id:  enterGrid
            model: enterList
            delegate:  letterDelegate
            height: keyHeight
            width: keyWidth*2
            interactive: false


            anchors
            {
                top: fourthRow.top
                bottom: fourthRow.bottom
                left: fourthLine.right
            }

        }
    }  // End Fourth Row of Keys


    // Fifth Row of Keys (space key)
    // At key
    ListModel
    {
        id: atList
        ListElement {lower: "@"; upper: "_" ; fzr: 1; kt: "n"; rw: 1}
    }

    // layout at key
    GridView
    {
        width: keyWidth; height: keyHeight
        id:  atGrid
        model: atList
        delegate:  letterDelegate
        interactive: false

        anchors
        {
            top: fourthRow.bottom
        }
    }


    // Space Key
    ListModel
    {
        id: spaceList
        ListElement {lower: " "; upper: " " ; fzr: 1; kt: "n"; rw: 8}
    }
    // layout space key
    GridView
    {
        width: keyWidth*8; height: keyHeight
        id:  spaceGrid
        model: spaceList
        delegate:  letterDelegate
        interactive: false

        anchors
        {
            top: fourthRow.bottom
            left: atGrid.right
        }
    }

    // Period Key
    ListModel
    {
        id: periodList
        ListElement {lower: "."; upper: "," ; fzr: 1; kt: "n"; rw: 1}
    }
    // layout Period key
    GridView
    {
        width: keyWidth; height: keyHeight
        id:  periodGrid
        model: periodList
        delegate:  letterDelegate
        interactive: false

        anchors
        {
            top: fourthRow.bottom
            left:spaceGrid.right
        }
    }

    // Current textstring display
    Rectangle {
        id: keyboardPrompt
        width: root.keyWidth
        height: root.keyHeight
        color: "black"
        anchors.bottom: root.bottom
        Text {
            id: textDisp
            anchors.centerIn: parent
            text: ""
            font.pointSize: 32
            color: "white"
        }
    }

    Rectangle {
        id: keyboardTextWrapper
        width: root.width - root.keyWidth
        height: root.keyHeight
        color: "black"//"#6cf"
        anchors.left: keyboardPrompt.right
        anchors.bottom: root.bottom
        TextEdit {
            id: keyboardText
            objectName : "keyboardText"
            anchors.centerIn: parent
            text: ""
            font.pointSize: 40
            color: "white"
        }
    }

    function letterTyped(key) {
        keyboardText.text += key
//        console.log(keyboardText.text)
    }
    function deletePressed() {
        keyboardText.text = keyboardText.text.substring(0, keyboardText.text.length-1)
//        console.log(keyboardText.text)
    }

    property alias keyboardString: keyboardText.text

    // Confirmation popup
//    onEnterKeySignal: {
//        confirmationPopup.opacity = 1;
//    }
/*
    Rectangle {
        id: confirmationPopup
        anchors.fill: parent
        color: "black"
        opacity: 0
        Behavior on opacity { PropertyAnimation { duration: 250 } }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
        }
        property int layoverAnimationDuration: 1000
        property int buttonTimerDuration: 1000

        Rectangle {
            id: submitButton
            anchors.centerIn: parent
            width: 100; height: 100
            anchors.horizontalCenterOffset: -100
            color: "green"
            border.width: 2
            border.color: "lightGrey"
            radius: 15
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    confirmationPopup.layoverAnimationDuration = 1000;
                    submitLayover.height = submitButton.height - submitButton.border.width;
                    submitTimer.start();
                }
                onExited: {
                    confirmationPopup.layoverAnimationDuration = 0;
                    submitLayover.height = 0;
                    submitTimer.stop();
                }
            }
            Text {
                anchors.centerIn: parent
                color: "white"
                text: "SUBMIT"
                font.pointSize: 14
                font.weight: Font.Bold
            }
            Rectangle {
                id: submitLayover
                x: parent.border.width/2
                y: x
                height: 0
                width: parent.width - submitButton.border.width
                opacity: 0.7
                color: "black"
                radius: parent.radius
                Behavior on height {
                    PropertyAnimation {
                        duration: confirmationPopup.layoverAnimationDuration
                    }
                }
            }
            Timer {
                id: submitTimer
                interval: confirmationPopup.buttonTimerDuration
                onTriggered: {
                    keyboardConfirmationSubmitted();    //signal [main]
                    root.hideKeyboard();
                    confirmationPopup.opacity = 0;
                }
            }
        }
        Rectangle {
            id: cancelButton
            anchors.centerIn: parent
            width: submitButton.width; height: submitButton.height
            anchors.horizontalCenterOffset: 100
            color: "red"
            border.width: submitButton.border.width
            border.color: submitButton.border.color
            radius: submitButton.radius
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    cancelTimer.start();
                    confirmationPopup.layoverAnimationDuration = 1000;
                    cancelLayover.height = submitButton.height - submitButton.border.width;
                }
                onExited: {
                    cancelTimer.stop();
                    confirmationPopup.layoverAnimationDuration = 0;
                    cancelLayover.height = 0;
                }
            }
            Text {
                anchors.centerIn: parent
                color: "white"
                text: "CANCEL"
                font.pointSize: 14
                font.weight: Font.Bold
            }
            Rectangle {
                id: cancelLayover
                x: parent.border.width/2
                y: x
                height: 0
                width: parent.width - submitButton.border.width
                opacity: 0.7
                color: "black"
                radius: parent.radius
                Behavior on height {
                    PropertyAnimation {
                        duration: confirmationPopup.layoverAnimationDuration
                    }
                }
            }
            Timer {
                id: cancelTimer
                interval: confirmationPopup.buttonTimerDuration
                onTriggered: {
                    root.hideKeyboard();
                    confirmationPopup.opacity = 0;
                }
            }
        }

    }

*/

}
