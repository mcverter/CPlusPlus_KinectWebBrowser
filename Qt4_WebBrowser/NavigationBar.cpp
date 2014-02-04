/* Filename: NavigationBar.cpp
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


#include "NavigationBar.h"
/*****************************
 *  METHOD: NavigationBar::NavigationBar (QWidget * parent)
 *  PRE:  Called by BrowserTab constructor
 *  POST:  Constructor
 ********************************/
NavigationBar::NavigationBar (QWidget * parent)
    : QWidget (parent)
{
    // create the GUI elements
    back = new QPushButton(QIcon("icons/backward.png"), "", this);
    fwd = new QPushButton(QIcon("icons/forward.png"),"", this);
    reload = new QPushButton(QIcon("icons/reload.png"), "", this);
    go = new QPushButton(QIcon("icons/go.png"), "", this);
    history = new QPushButton(QIcon("icons/history.png"), "", this);

    home = new QPushButton(QIcon("icons/home.ico"), "", this);



    addressBar = new CompleterLineEdit(this);
    addressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // layout

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->addWidget(back);
    mainLayout->addWidget(fwd);
    mainLayout->addWidget(home);
    mainLayout->addWidget(history);
    mainLayout->addWidget(reload);
    mainLayout->addWidget(addressBar);
    mainLayout->addWidget(go);
    setLayout(mainLayout);

    // Forwarding signals from buttons
    connect(back,SIGNAL(clicked()),this,SIGNAL(backPressedSignal()));
    connect(fwd,SIGNAL(clicked()),this,SIGNAL(fwdPressedSignal()));
    connect(reload,SIGNAL(clicked()),this,SIGNAL(reloadPressedSignal()));

    connect(home,SIGNAL(clicked()),this,SLOT(goToHomeSlot()));
    connect(go,SIGNAL(clicked()),this,SLOT(goToPageSlot()));
    connect(addressBar,SIGNAL(returnPressed()),this,SLOT(goToPageSlot()));

    // Passes signals to and from Virtual Keyboard
    connect(addressBar, SIGNAL(lineEditClicked()), this, SIGNAL(lineEditClicked()));
    connect(this, SIGNAL(wordEntered(QString)), addressBar, SLOT(addWordSlot(QString)));

    QTimer::singleShot(0, addressBar, SLOT(setFocus()));
}


/*****************************
 *  METHOD: handleKinectMouse(float kinect_x, float kinect_y)
 *  PRE:  Called by BrowserTab constructor
 *  POST:  Constructor
 ********************************/



bool NavigationBar::checkGlobalPoint(QWidget * widge, int kinect_x, int kinect_y)
{
 //   dumpRelativeCoordinates(widge, kinect_x, kinect_y);
    QPoint UL = this->mapToGlobal(widge->geometry().topLeft());
    QPoint LR = this->mapToGlobal(widge->geometry().bottomRight());

    qDebug() <<  "point= (" << kinect_x << ", " << kinect_y << ") " << widge->metaObject()->className() << " " << UL << "  "  << LR;
    if (kinect_x >= UL.x() && kinect_x <= LR.x() &&
            kinect_y >= UL.y() && kinect_y <= LR.y())
    {
        return true;
    }
    return false;
}
void NavigationBar::handleKinectMouse(int kinect_x, int kinect_y)
{
    if (checkGlobalPoint(this, kinect_x, kinect_y))
    {

    if (checkGlobalPoint(addressBar, kinect_x, kinect_y))
        {
            emit lineEditClicked();

        }
    else if (checkGlobalPoint(history, kinect_x, kinect_y))
        {
            emit historyClicked();
        }
    else if (checkGlobalPoint(reload, kinect_x, kinect_y))
        {
            emit reloadPressedSignal();
        }

    else if (checkGlobalPoint(back, kinect_x, kinect_y))
        {
            emit backPressedSignal();
        }
    else if (checkGlobalPoint(home, kinect_x, kinect_y))
        {
            goToHomeSlot();
        }
    else if (checkGlobalPoint(go, kinect_x, kinect_y))
        {
            goToPageSlot();
        }
    else if (checkGlobalPoint(fwd, kinect_x, kinect_y))
        {
            emit fwdPressedSignal();
        }
    }
}


/*****************************
 *  METHOD: void NavigationBar::getAddressBar()
 *  PRE:   Called by BrowserTab constructor
 *  POST: Used to wire together changes in browser history to address bar
 ********************************/

CompleterLineEdit * NavigationBar::getAddressBar()
{
    return addressBar;
}

/*****************************
 *  METHOD:
 *  PRE:
 *  POST:
********************************/

QPushButton * NavigationBar::getHistoryButton()
{
    return history;
}

/*****************************
 *  METHOD: void NavigationBar::goToHome()
 *  PRE:   HOME Button pressed
 *  POST: locationChanged signal emitted
 *                  with argument "http://www.curiomotion.com"
 ********************************/
void NavigationBar::goToHomeSlot()
{
    emit locationChangedSignal(QUrl("http://www.curiomotion.com"));
}
/*****************************
 *  METHOD: void NavigationBar::goToPage()
 *  PRE:  GO button pressed or RETURN entered in AddressBar
 *  POST: locationChanged signal emitted
 ********************************/

void NavigationBar::goToPageSlot()
{
    QString webAddress = addressBar->text();
    webAddress = normalizeURLString(webAddress);
    QUrl webUrl = webAddress;
    emit locationChangedSignal(webUrl);
}

/*****************************
 *  METHOD: void NavigationBar::onBrowserURLChangedSlot(const QUrl &url)
 *  PRE:   Navigation occurs from within the browser
 *  POST:  Resets string in AddressBar
 ********************************/
void NavigationBar::onBrowserURLChangedSlot(const QUrl &url)
{  
    QRegExp rx("#\!.*");
    QString  urlAsString = url.toString();
    urlAsString.replace(rx, "");
    addressBar->setText(urlAsString);
    addressBar->setCursorPosition(0);
}


/*****************************
 *  METHOD: QString normalizeURLString(const QString rawInput)
 *  PRE:  URL entered in address bar and return key or Go button pressed
 *  POST:   Well formed URL
 ********************************/
QString NavigationBar::normalizeURLString(const QString rawInput)
{
    QString normalizedString = rawInput;

    QRegExp protocolRegX("http://.+");

    if(! protocolRegX.exactMatch(rawInput))
    {
        normalizedString = "http://" + normalizedString;
    }

    QRegExp domainRegX(".+\\.(aero|asia|biz|cat|com|coop|info|int|jobs|mobi|museum|name|net|org|post|pro|tel|travel|xxx|edu|gov|mil|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|ax|az|ba|bb|bd|be|bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cs|cu|cv|cx|cy|cz|dd|de|dj|dk|dm|do|dz|ec|ee|eg|eh|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|rs|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|ss|st|su|sv|sx|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zw).*");

    if(! domainRegX.exactMatch(rawInput))
    {
        normalizedString =  normalizedString + ".com";
    }
    return normalizedString;
}


/*****************************
 *  METHOD: NavigationBar::~NavigationBar (){}
 *  PRE:
 *  POST: Destroy NavigationBar object
 ********************************/
NavigationBar::~NavigationBar (){}
