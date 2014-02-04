/* Filename: CurioWebView.cpp
 * 
 * Objects:  (1) CurioWebView extends QWebView
 *            
 *  The CurioWebView contains the browser 
 *
 * It can go back, forward, change url,
 * 
 */


#include "CurioWebView.h"


/*****************************
 *  METHOD:  CurioWebView::CurioWebView(QWidget *parent) :
 *  PRE:
 *  POST:  Constructor
 ********************************/ 
CurioWebView::CurioWebView(QWidget *parent, ModalKeyboard * keyboard) :
    QWebView(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(urlChangedSlot(bool)));
    tabKeyboard = keyboard;
}

/*****************************
 *  METHOD:  CurioWebView::handleKinectMouse(bool)
 *  PRE:  kinect hovers over point in webview for threshold amount of time
 *  POST: Either activates the scroll bar or sends a mouse click to the
 *       Web Page contained within the webview
********************************/
void CurioWebView::handleKinectMouse(int kinect_x, int kinect_y)
{

    // check scroll bar
    QRect scrollBarGeometry = this->page()->currentFrame()->scrollBarGeometry(Qt::Vertical);

    if (scrollBarGeometry.contains(QPoint(kinect_x, kinect_y)))
    {
        this->page()->currentFrame()->setScrollPosition(QPoint(kinect_x, kinect_y));
    }

    else
    {
    this->setFocus();

    QMouseEvent press = QMouseEvent(QEvent::MouseButtonPress,
                                    this->mapFromGlobal(QPoint(kinect_x, kinect_y)),
                                    Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

    QMouseEvent release = QMouseEvent(QEvent::MouseButtonRelease,
                                    this->mapFromGlobal(QPoint(kinect_x, kinect_y)),
                                    Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);


    QApplication::sendEvent( this, &press);
    QApplication::sendEvent(this, &release);
    QApplication::sendEvent( this, &press);
    QApplication::sendEvent(this, &release);

    }
}

/*****************************
 *  METHOD:  CurioWebView::urlChangedSlot(bool)
 *  PRE:  called when url changed
 *  POST: emits url and title.  Needed to record history
          Uses Javascript to adds Tags to the input boxes so
          that they open up the ModalKeyboard
********************************/
void CurioWebView::urlChangedSlot(bool finished)
{
    if (finished==true)
    {
        this->page()->mainFrame()->addToJavaScriptWindowObject("webbridge", tabKeyboard);
        const QString script = "var inputs = document.getElementsByTagName('INPUT');var index;for(index=0; index < inputs.length; index++){if (inputs[index].type=='text' || inputs[index].type=='textarea' || inputs[index].type=='password' || inputs[index].type=='email' || inputs[index].type=='tel' ||inputs[index].type=='url'){ inputs[index].onclick = function(){this.value=webbridge.openInputContext(this.value);}}}";
        this->page()->mainFrame()->evaluateJavaScript(script);

        emit webPageVisited(this->url(), this->title());
    }
}

/*****************************
 *  METHOD:  CurioWebView::changeTitle()
 *  PRE: Called when History Tab is opened
 *  POST:  titleChanged("History") signal is emitted
********************************/

void CurioWebView::changeTitle(const QString & title)
{
    emit titleChanged(title);
}

/*****************************
 *  METHOD: void CurioWebView::contextMenuEvent ( QContextMenuEvent * ev )
 *  PRE:  Called when someone right clicks within the WebView
 *  POST:  If the click is over an image, "Save Image" appears.  
 *                Otherwise, "Back" "Fwd" "Reload" appear.
 ********************************/
void CurioWebView::contextMenuEvent ( QContextMenuEvent * ev )
{
    QMenu * menu = new QMenu(this);

    QWebHitTestResult htr =
            this->page()->mainFrame()->hitTestContent(ev->pos());

    if ((!(htr.imageUrl().isEmpty())))
    {
        menu->addAction(pageAction(QWebPage::DownloadImageToDisk));
    }
    else
    {
        menu->addAction(pageAction(QWebPage::Back));
        menu->addAction(pageAction(QWebPage::Forward));
        menu->addAction(pageAction(QWebPage::Reload));
    }
    menu->exec(ev->globalPos());
}


/*****************************
 *  METHOD:
 *  PRE:
 *  POST:  Destructor
********************************/

CurioWebView::~CurioWebView()
{}
