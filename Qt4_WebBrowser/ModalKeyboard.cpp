/*
Modal Keyboard

The Modal Keyboard is a dialog which solicits user
input usin a QML Virtual Keyboard.

It blocks all input until the ENTER key is pressed


*/

#include "ModalKeyboard.h"
#include <QDebug>
#include <QDialog>
#include <QEventLoop>

void ModalKeyboard::resizeEvent(QResizeEvent * e)
{
    QSize  sz = e->size();
    keyboardObject->setProperty("width", sz.width());
    keyboardObject->setProperty("height", sz.height());
    keyboard->show();
}


// Constructor
ModalKeyboard::ModalKeyboard(QWidget *parent) :
    QWidget(parent)
{

    keyboard = new QDeclarativeView(this);

   keyboard->setSource(QUrl::fromLocalFile("qml/main.qml"));
    keyboardObject = keyboard->rootObject();
    QObject::connect((QObject*)keyboardObject, SIGNAL (wordCompleted(QString)), this, SLOT(enterReceivedSlot(QString)));

   connect(this, SIGNAL(accepted()), this, SLOT(setKeyboardStringSlot()));
}

// Taken from WebBridge
// Input string is set when keyboard Dialog closed
void ModalKeyboard::setKeyboardStringSlot()
{

    inputString = getTyped();
    setTyped("");
}

// Called by Javascript in web Page
// Opens keyboard and returns keyboard string
QString ModalKeyboard::openInputContext(const QString &orgtext)
{

  setTyped(orgtext);
  exec();
  return inputString;
}



QString ModalKeyboard::getTyped()
{
    return typed;
}

void ModalKeyboard::setTyped(QString input)
{
    typed = input;
}

// shows the Keyboard and sets the start string of the display
int ModalKeyboard::exec()
{ 
    emit opened();
    QObject * keyboardValueLabel = ((QObject*)keyboardObject)->findChild<QObject *>("keyboardText");
    keyboardValueLabel->setProperty("text", typed);
  //  QQmlProperty(keyboardValueLabel, "text").write(typed);
    QEventLoop loop;
    QObject::connect(this, SIGNAL(accepted()), &loop, SLOT(quit()));

    loop.exec();
}


// Slot called when the user presses RETURN key
// Sets the typed variable .
// Closes keyboard and emits accepted()
void ModalKeyboard::enterReceivedSlot(QString word)
{
    typed = word;
    keyboard->close();
    emit accepted();
}

