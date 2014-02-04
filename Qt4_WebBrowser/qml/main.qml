import QtQuick 1.0
// on click a button, virtual key board show
// on enter from keyboardk,  virtual keyboard hidden
// have a textfield somewherer that you modify with the key presses from the keyboard

Rectangle
{
   id:  thebigbopper
   //height: 450
   //width:  900
   color: "red"


   signal letterTyped(string key)
   signal deletePressed()
   signal wordCompleted(string word)


    Rectangle
    {
        id: keyboardcontainer
        height:  parent.height
        width:  parent.width
        VirtualKeyboard
        {
            id: virtualKB
            onNormalKeySignal :
            {
                letterTyped(key)
            }
            onBackspaceKeySignal:
            {
                deletePressed()
            }

            onEnterKeySignal:
            {
                wordCompleted(keyboardString)
                keyboardString=""
            }


        }
    }

}
