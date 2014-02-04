/* Filename: NavigationBar.h
 * 
 * Objects:  (1) NavigationBar extends QWidget
 *            
 * The NavigationBar is the top widget of the browser.
 *
 * It contains:
 * (a) Back Button
 * (b) Fwd Button
 * (c) Reload Button
 * (d) Home Button
 * (e) Go Button
 * (f) Address Bar 
 *
 */



#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H


#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QUrl>
#include <QCompleter>
#include <QTimer>
#include <QDebug>
#include "CompleterLineEdit.h"


class NavigationBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationBar(QWidget *parent = 0);
    ~NavigationBar();
    CompleterLineEdit * getAddressBar();
    QPushButton * getHistoryButton();
     void handleKinectMouse(int, int);


  signals:
    void backPressedSignal() ;
    void fwdPressedSignal() ;
    void reloadPressedSignal();
    void locationChangedSignal(QUrl);
    void lineEditClicked();
    void wordEntered(QString);
    void historyClicked();

private slots:
     void goToPageSlot();
     void goToHomeSlot();
     void onBrowserURLChangedSlot(const QUrl &arg1);

private:

// GUI elements 
   CompleterLineEdit *addressBar;
   QPushButton * back;
   QPushButton * fwd;
   QPushButton * go;
   QPushButton * home;
   QPushButton * reload;
   QPushButton * history;

   QString normalizeURLString(const QString);   
   bool checkGlobalPoint(QWidget * , int, int);
};

#endif //  NAVIGATIONBAR_H
