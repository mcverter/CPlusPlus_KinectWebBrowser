/* Filename: BrowserTab.cpp
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
 * It contains:
 *    (a) TabWidget
 *    (b) CurioBrowsers
 *    (c) Objects for saving to local filesystem
 *        (i) FileDialog
 *        (ii) QStorageInfo
 *        (iii) QErrorMessage
 */


#include "BrowserTab.h"

// Static History shared by all BrowserTabs
History * BrowserTab::mHistory=0;

BrowserTab::~BrowserTab()
{
}

/*****************************
 *  METHOD: BrowserTab::BrowserTab(QWidget *parent)
 *  PRE:  A BrowserTab can be initially opened as a browser tab or as a history tab.
 *  POST:  Constructor. GUI and FileStorage Components are initialized.
 *         Static history is initialized.
 *         Either homepage or history is opened
********************************/
BrowserTab::BrowserTab(QWidget *parent,
                       bool historyTab)
    : QWidget (parent)
{
    // creates the static history object if it doesn't already exist
    if (mHistory==0)
    {
        mHistory=new History();
    }


    initializeGUI();

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // either opens a history tab or loads the homepage
    if (historyTab)
    {
        QString historyHTML = mHistory->outputHTML();
        browser->setHtml(historyHTML);
        browser->changeTitle("History");
    }
    else
    {
        browser->load(QUrl("http://www.google.com"));
    }
}

/*****************************
 *  METHOD: BrowserTab::initializeGUI()
 *  PRE: Called by Constructor
 *  POST:  GUI components initialized and signals connected
 ********************************/
void BrowserTab::initializeGUI() {
    // create the GUI elements

    mKeyboard = new ModalKeyboard(this);

    browser = new CurioWebView(this, mKeyboard);
    nav = new NavigationBar(this);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(nav);
    mainLayout->addWidget(browser);

    mainLayout->addWidget(mKeyboard);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);

    // initialize browser height for keyboard show & hide
    webViewHeight = browser->height();


    // handle NavigationBarEvents
    connect(nav,SIGNAL(backPressedSignal()),browser,SLOT(back()));
    connect(nav,SIGNAL(fwdPressedSignal()),browser,SLOT(forward()));
    connect(nav,SIGNAL(reloadPressedSignal()),browser,SLOT(reload()));
    connect (nav, SIGNAL(locationChangedSignal(QUrl)), this, SLOT(onNavLocationChangedSlot(QUrl)));

    connect(mHistory, SIGNAL(historyUpdatedSignal(QStringList)), nav->getAddressBar(), SLOT(setMatchListSlot(QStringList)));


    // handle url change within the web view
    connect (browser, SIGNAL(urlChanged(QUrl)), nav, SLOT(onBrowserURLChangedSlot(QUrl)));
    connect (browser, SIGNAL(webPageVisited(QUrl,QString)), mHistory, SLOT(addToHistorySlot(QUrl,QString)));
    connect (browser, SIGNAL(urlChanged(QUrl)), this, SLOT(hideKeyboard()));


    connect (nav, SIGNAL(lineEditClicked()), this, SIGNAL(lineEditClicked()));

    // passes signal from virtual keyboard
    connect(this, SIGNAL(wordEntered(QString)), nav, SIGNAL(wordEntered(QString)));


    connect (nav, SIGNAL(historyClicked()), this, SIGNAL(historyClicked()));
    connect(mKeyboard,SIGNAL(opened()) , this, SLOT(showKeyboard()));
    connect (mKeyboard, SIGNAL(accepted()), this, SLOT(hideKeyboard()));



    // To transfer VirtualKeyboard Input to address bar
    connect(nav, SIGNAL(lineEditClicked()), this, SLOT(setAddressBar()));
}

/*****************************
 *  METHOD: BrowserTab::setAddressBar()
 *  PRE: Called when address Bar is clicked
 *  POST:  Transfers VirtualKeyboard Input to address bar
 ********************************/
void BrowserTab::setAddressBar()
{
    QString typed = mKeyboard->openInputContext(nav->getAddressBar()->text());
    emit wordEntered(typed);
}

/*****************************
 *  METHOD: BrowserTab::resizeEvent()
 *  PRE: Called when browser is resized
 *  POST:  Resets web view height.  Needed for showing & hiding keyboard
 ********************************/
void BrowserTab::resizeEvent(QResizeEvent *)
{
    webViewHeight = browser->height();
}


/*****************************
 *  METHOD: BrowserTab::BrowserTab(QWidget *parent)
 *  PRE:  A BrowserTab can be initially opened as a browser tab or as a history tab.
 *  POST:  Constructor. GUI and FileStorage Components are initialized.
*****************************/
void BrowserTab::saveHistory()
{
    mHistory->saveToFile();
}




/*****************************
 *  METHOD: processKinectMouse(int kinect_x, int kinect_y)
 *  PRE:  Called by MainWindow.cpp in response to Kinect Mouse Hover
 *  POST:  Depending on location, click is either
 *       (1) Ignored
 *       (2)  Passed to Navigation Bar
 *       (3)  Passed to Web view
 ********************************/

void BrowserTab::processKinectMouse(int kinect_x , int kinect_y)
{
    if (this->geometry().contains(this->mapFromGlobal(QPoint(kinect_x, kinect_y))))
    {
        if (nav->geometry().contains(nav->mapFromGlobal(QPoint(kinect_x, kinect_y))))
        {
            nav->handleKinectMouse(kinect_x, kinect_y);
        }
        else  if (browser->geometry().contains(browser->mapFromGlobal(QPoint(kinect_x, kinect_y))))
        {
            browser->handleKinectMouse(kinect_x, kinect_y);
        }


    }
}


/*****************************
 *  METHOD: BrowserTab::getBrowser()
 *  PRE:  Called in Main to wire together a change in url title to change in tab title
 *  POST:   Returns the browser
*****************************/
CurioWebView* BrowserTab::getBrowser()
{
    return browser;
}


// Retrieves Nav Bar
NavigationBar* BrowserTab::getNav()
{
    return nav;
}

/*****************************
*  METHOD: void  BrowserTab::getHistoryButton()
*  PRE: Called by MainWindow
*  POST: Returns History Button from navigation bar
 ********************************/

QPushButton * BrowserTab::getHistoryButton()
{
    return nav->getHistoryButton();
}

/************
 *  SLOTS
 ************/

/*****************************
*  METHOD: void  BrowserTab::onNavLocationChanged(const QUrl &url)
*  PRE: User hits return or GO in Navigation Bar 
*  POST: Web Browswer displays new page
 ********************************/
void BrowserTab::onNavLocationChangedSlot(const  QUrl &url) {
    browser->load(url);
    browser->show();
}


/*****************************
*  METHOD: void  BrowserTab::hideKeyboard()
*  PRE: User hits ENTER on VirtualKeyboard
*  POST: Keyboard is hidden and Webview fully shown
 ********************************/
void BrowserTab::hideKeyboard()
{

   QRect browGeo = browser->geometry();
   browser->setGeometry(browGeo.x(), browGeo.y(), browGeo.width() , webViewHeight * webViewShrinkRatio);
   mKeyboard->hide();
}

/*****************************
*  METHOD: void  BrowserTab::showKeyboard()
*  PRE: User clicks on Javascript dialogue or Address Bar
*  POST: Keyboard is fully shown and Webview is half-hidden
 ********************************/

void BrowserTab::showKeyboard()
{
    QRect browGeo = browser->geometry();
    browser->setGeometry(browGeo.x(), browGeo.y(), browGeo.width() , webViewHeight/webViewShrinkRatio);
    mKeyboard->move(0,browGeo.y() + webViewHeight/webViewShrinkRatio);
    mKeyboard->resize(this->width(), webViewHeight/webViewShrinkRatio); // - 10);
    mKeyboard->show();
} 



