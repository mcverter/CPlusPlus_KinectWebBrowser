/* Filename: BrowserTab.h
 * 
 * Objects:  (1) BrowserTab
 *            
 *  The Browser Tab contains a tab-set, 
 *      each tab of which contains a CurioBrowser.
 *
 *  It is responsible for passing signals and slots
 *     to and from the Browsers within the tabs. 
 *
*  Each Browser tab also controls the downloading 
 *       of images to the local filesystem 
 * 
*
*    The various instances of the BrowserTab class
*       share a static History object between them
 */


#ifndef BROWSERTAB_H
#define BROWSERTAB_H


#include <QLayout>
#include <QUrl>
#include <QDebug>

#include "CurioWebView.h"
#include "NavigationBar.h"
#include "History.h"


class BrowserTab : public QWidget
{
    Q_OBJECT

 public:
  explicit BrowserTab(QWidget *parent = 0,
                      bool historyTab = false);
  ~BrowserTab();
 CurioWebView * getBrowser();
 NavigationBar * getNav();

 // Static history object accessible to MainWindow
 static History * mHistory;
 QPushButton * getHistoryButton();
 void saveHistory();

 // redirects kinect mouse to navbar or webview
 void processKinectMouse(int, int);

signals:
 void lineEditClicked();
 void wordEntered(QString);
 void deletePressed();
 void historyClicked();


public slots:

  // for communicating with NavigationBar
  void onNavLocationChangedSlot(const QUrl &url);

  // for showing and hiding keyboard
  void showKeyboard();
  void hideKeyboard();
    // for setting address bar with virtual keyboard
  void setAddressBar();


 private:

  void initializeGUI();

  // basic web browsing
  NavigationBar * nav;
  CurioWebView * browser;

    // modal keyboard controls
    ModalKeyboard * mKeyboard;
    // for showing and hiding keyboard
    int webViewHeight;
    static const int webViewShrinkRatio = 2;



protected:
    void resizeEvent(QResizeEvent * e);
};


#endif //  BROWSERTAB_H
