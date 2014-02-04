#ifndef MODALKEYBOARD_H
#define MODALKEYBOARD_H

#include <QObject>
#include <QWidget>
#include <QtDeclarative/QtDeclarative>

class ModalKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit ModalKeyboard(QWidget *parent = 0);
    int exec();
    QString getTyped();
    void setTyped(QString);
    void hideKeyboard();

signals:
    void accepted();
    void opened();

public slots:
    void enterReceivedSlot(QString);
    void setKeyboardStringSlot();
    QString openInputContext(const QString&);

protected:
    void resizeEvent(QResizeEvent * e);

private:
    // containers for keyboard
    QWidget * widge ;

    // keyboard
    QDeclarativeView * keyboard;
    QObject * keyboardObject;

    // internal representations of typed string
    QString typed;
    QString inputString;
};

#endif // MODALKEYBOARD_H
