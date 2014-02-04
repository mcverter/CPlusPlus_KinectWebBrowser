#include <QDebug>

/* Filename: CompleterLineEdit.cpp
 *
 * Objects:  (1) CompleterLineEdit
 *
 *  The CompleterLineEdit is a LineEdit
*      which enables line completion.
*
*/

#include "CompleterLineEdit.h"
/*****************************
 *  METHOD: CompleterLineEdit::CompleterLineEdit(QWidget* parent)
 *  PRE:  Called from naviation bar constructor.
 *  POST:  Constructor.
********************************/


CompleterLineEdit::CompleterLineEdit(QWidget *parent) :
    QLineEdit(parent)
{    
    words =  QStringList() << "";
    listView = new QListView(this);
    model = new QStringListModel(this);
    listView->setWindowFlags(Qt::ToolTip);

    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(findMatchesSlot(const QString &)));
    connect(listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(completeTextSlot(const QModelIndex &)));
}

/*****************************
 *  METHOD:  CompleterLineEdit::findMatchesSlot(text)
 *  PRE:  A letter has been entered into the line edit
 *  POST:  Possible matches for the current string are found
********************************/

void CompleterLineEdit::findMatchesSlot(const QString & text)
{
    if (text.isEmpty()) {
        listView->hide();
        return;
    }

    if ((text.length() > 1) && (!listView->isHidden())) {
        return;
    }


    // If you complete a word in the list that contains the input text is added to display the complete list of string

    QStringList sl;
    foreach(QString word, words) {
        if (word.contains(text)) {
            sl << word;
        }
    }

    model->setStringList(sl);
    listView->setModel(model);

    if (model->rowCount() == 0) {
        return;
    }

    // Position the text edit
    listView->setMinimumWidth(width());
    listView->setMaximumWidth(width());

    QPoint p(0, height());
    int x = mapToGlobal(p).x();
    int y = mapToGlobal(p).y() + 1;

    listView->move(x, y);
    listView->show();

}
/*****************************
 *  METHOD:  CompleterLineEdit::setMatchListSlot()
 *  PRE:  A new item has been added to the History object
 *  POST:  StringList of possible matches is updated
********************************/

void CompleterLineEdit::setMatchListSlot (const QStringList & matches)
{
    words = matches;
}
/*****************************
 *  METHOD:  CompleterLineEdit::completeTextSlot(const QModelIndex index)
 *  PRE: A completion is chosen
 *  POST:  The line edit is changed to the chosen completion
********************************/

void  CompleterLineEdit::completeTextSlot(const QModelIndex &index)
{
    QString text = index.data().toString();
    setText(text);
    listView->hide();
}
/*****************************
 *  METHOD:  CompleterLineEdit::keyPressEvent(e)
 *  PRE:  A key is pressed in the line edit
 *  POST:  Up/down key scrolls up/down in completion list
 *         Return key terminates selection event
 *         Character initiates a string match search
********************************/

void CompleterLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (!listView->isHidden()) {
        int key = e->key();
        int count = listView->model()->rowCount();
        QModelIndex currentIndex = listView->currentIndex();

        if (Qt::Key_Down == key) {

            // Press the down arrow key to move on to complete the next item in the list

            int row = currentIndex.row() + 1;
            if (row >= count) {
                row = 0;
            }

            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        } else if (Qt::Key_Up == key) {


            // Press the down arrow key to move the cursor to select a complete item in the list

            int row = currentIndex.row() - 1;
            if (row < 0) {
                row = count - 1;
            }

            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        } else if (Qt::Key_Escape == key) {


            // Pressing the Esc key, Hide completion list

            listView->hide();
        } else if (Qt::Key_Enter == key || Qt::Key_Return == key) {

    // When the Enter key is pressed, using the complete list of the selected items, and hide the list is complete

            if (currentIndex.isValid()) {
                QString text = listView->currentIndex().data().toString();
                setText(text);
            }

            listView->hide();
            QLineEdit::keyPressEvent(e);
        } else {


            // Otherwise, hide the list is complete and QLineEdit keyboard press event
            listView->hide();
            QLineEdit::keyPressEvent(e);
        }
    } else {
        QLineEdit::keyPressEvent(e);
    }

//    QLineEdit::keyPressEvent(e);
}

/*****************************
 *  METHOD:  CompleterLineEdit::focusOutEvent()
 *  PRE:  focus is removed from address bar
 *  POST:  Completion list is hidden
********************************/

void CompleterLineEdit::focusOutEvent(QFocusEvent *e){
    listView->hide();
}


/*****************************
 *  METHOD:  CompleterLineEdit::mouseReleaseEvent( )
 *  PRE:  mouse clicked in address bar
 *  POST: Virtual Keyboard is shown
********************************/
void CompleterLineEdit::mouseReleaseEvent(QMouseEvent * e)
{
    emit(lineEditClicked());
}

/*****************************
 *  METHOD:  CompleterLineEdit::mouseReleaseEvent( )
 *  PRE:  ENTER pressed in virtual keyboard
 *  POST: Address bar is set with contents of Virtual Keyboard
********************************/
void CompleterLineEdit::addWordSlot(const QString string)
{
        setText(string.trimmed());
}


