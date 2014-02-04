/* Filename: CompleterLineEdit.h
 *
 * Objects:  (1) CompleterLineEdit
 *
 *  The CompleterLineEdit is a subclassed LineEdit
*      which enables line completion.
*
*/



#ifndef COMPLETERLINEEDIT_H
#define COMPLETERLINEEDIT_H

#include <QLineEdit>
#include <QStringList>
#include <QListView>
#include <QStringListModel>
#include <QModelIndex>
#include <QKeyEvent>

class CompleterLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CompleterLineEdit(QWidget *parent = 0);

signals:
    void lineEditClicked();

public slots:
    void findMatchesSlot(const QString & text);
    void setMatchListSlot (const QStringList & matches);
    void completeTextSlot(const QModelIndex &index);
    void addWordSlot(const QString);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    QStringList words;
    QListView *listView;
    QStringListModel *model;
};

#endif // COMPLETERLINEEDIT_H
