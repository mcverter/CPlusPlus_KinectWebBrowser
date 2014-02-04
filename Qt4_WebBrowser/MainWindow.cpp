/* Filename: MainWindow.cpp
 * 
 * Objects:  (1) MainWindow extends QMainWindow
 *            
 *  The Main Window presents the main interface of the WebBrowser
 *
 * It contains:
 * (a) BrowserTab
 * (b) newTabButton
 */


#include "MainWindow.h"

/*****************************
 *  METHOD: MainWindow::MainWindow(QWidget * parent)
 *  PRE:  Called by main.cpp
 *  POST:  Constructor
 ********************************/
MainWindow::MainWindow(QWidget * parent)
  : QMainWindow (parent){

  // for multimedia support
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);

    tabs = new QTabWidget(this);
    tabs->setTabsClosable(true);
    tabs->addTab(new BrowserTab(tabs), "Untitled");

    newTabBtn = new QPushButton(QIcon("icons/addTab.png"), "", this);
    tabs->setCornerWidget(newTabBtn, Qt::TopLeftCorner);

    setCentralWidget(tabs);
    closeButton = new QPushButton(QIcon("icons/close.png"), "", this);

    tabs->setCornerWidget(closeButton, Qt::TopRightCorner);

    connect (newTabBtn, SIGNAL(clicked()), this, SLOT(addNewTabSlot()), Qt::UniqueConnection);
    connect(tabs,SIGNAL(tabCloseRequested(int)), this , SLOT(closeTabSlot(int)), Qt::UniqueConnection);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    // Events within the tab bar must be wired through MainWindow
    connect(((BrowserTab *) tabs->widget(0))->getBrowser(),
            SIGNAL(titleChanged(QString)),
            this,
            SLOT(setTabTitleSlot(QString)), Qt::UniqueConnection);

    connect(((BrowserTab *) tabs->widget(0))->getHistoryButton(),
            SIGNAL(clicked()),
            this,
            SLOT(addHistoryTabSlot()), Qt::UniqueConnection);

    connect(((BrowserTab *) tabs->widget(0)),
            SIGNAL(historyClicked()),
            this,
            SLOT(addHistoryTabSlot()), Qt::UniqueConnection);



    connect (tabs, SIGNAL(currentChanged(int)), this, SLOT(connectBrowserToTabTitleSlot(int)), Qt::UniqueConnection);

    //Set up Kinect User Interface
    kreader.start();
    xnFPSInit(&xnFPS, 180);
    mCalibrated = FALSE;
    connect(&kreader,SIGNAL(dataNotification()), this, SLOT(kinectLocationSlot()));
    connect(&kreader,SIGNAL(statusNotification(QKinect::KinectStatus)),this,SLOT(kinectStatusSlot(QKinect::KinectStatus)));
    connect(&kreader,SIGNAL(userNotification(unsigned,bool)),this,SLOT(kinectUserSlot(unsigned,bool)));
    connect(&kreader,SIGNAL(poseNotification(unsigned,QString)),this,SLOT(kinectPoseSlot(unsigned,QString)));
    connect(&kreader,SIGNAL(calibrationNotification(unsigned,QKinect::CalibrationStatus)),this,SLOT(kinectCalibrationSlot(unsigned,QKinect::CalibrationStatus)));

    /*
    // screen to kinect ratios
    screenH = 1024.0 ;
    screenV = 768.0;
    kinectH = 640.0;
    kinectV = 480.0;

    ratioH = 4;//screenH/kinectH;
    ratioV = 4;//screenH/kinectH;
*/
    // Use the SMA to stabilize the mouse
    xAVG = new SMA();
    yAVG = new SMA();
    for (int i=0;i<=8;i++) {xAVG->add(0.0); yAVG->add(0.0);}
    setWindowState(Qt::WindowFullScreen);

}

// prints out kinect status
// only useful for debugging

void MainWindow::kinectStatusSlot(QKinect::KinectStatus s)
{

  QString str("Kinect: ");
  if(s==QKinect::Idle)
    str += "Idle";
  if(s==QKinect::Initializing)
    str += "Initializing";
  if(s==QKinect::OkRun)
    str += "Running";
  if(s==QKinect::ErrorStop)
    str += "Error";
  qDebug()<<str;
}

// prints out user pose
// only useful for debugging
void MainWindow::kinectPoseSlot(unsigned id,QString pose)
{
  printf("User %u: Pose '%s' detected\n",id,pose.toStdString().c_str());
  qDebug() << "User " << id << " Pose " << pose;

}

// prints out presence of users
// only useful for debugging
void MainWindow::kinectUserSlot(unsigned id, bool found)
{
  printf("User %u: %s\n",id,found?"found":"lost");
  qDebug() << "User Found:  "  << found;
}


// prints out whether user has calibrated her pose
// only useful for debugging
void MainWindow::kinectCalibrationSlot(unsigned id, QKinect::CalibrationStatus s)
{
  printf("User %u: Calibration ",id);
  if(s==QKinect::CalibrationStart)
  {
    qDebug() << " Calibration Started ";
    printf("started\n");
  }
  if(s==QKinect::CalibrationEndSuccess)
  {
      qDebug() << " Calibration Successful ";
      printf("successful\n");
    mCalibrated = TRUE;
  }
  if(s==QKinect::CalibrationEndFail)
  {
      qDebug() << " Calibration Failed ";
    printf("failed\n");
  }
}




/*****************************
 *  METHOD: void MainWindow::kinectLocationSlot()
 *  PRE:  Responds to the Kinect Data response
 *  POST:  Moves the mouse in accordance with the user's Right Hand
 *         If the hand hovers for over two seconds, a mouse click is simulated
 ********************************/
void MainWindow::kinectLocationSlot()
{


    float avgX;
    float avgY;

    QImage * cursorImgPtr;
    QKinect::Bodies bodies = kreader.getBodies();
    int numBodies = bodies.size();
    qint64 kinectTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    waverThreshold_X = waverThreshold_Y = 10;
 // wait until a calibrated body has been detected
    if (numBodies < 1 || mCalibrated==FALSE) {return;}

    currentActiveBody = bodies[numBodies-1];


    //Save previous values in case we ignore hand move
    previousActiveHandPos.X = xAVG->avg();
    previousActiveHandPos.Y = yAVG->avg();

    //set Current hand
    if (currentActiveBody.proj_joints[QKinect::LeftHand].Y < currentActiveBody.proj_joints[QKinect::RightHand].Y)
    {
      activeHandPos = currentActiveBody.proj_joints[QKinect::LeftHand];
      cursorImgPtr = &leftCursor;

    }
    else
      {
        activeHandPos = currentActiveBody.proj_joints[QKinect::RightHand];
        cursorImgPtr = &rightCursor;
      }

    // Use the SMA to stabilize the mouse
    xAVG->add(activeHandPos.X);
    yAVG->add(activeHandPos.Y);
    avgX = xAVG->avg();
    avgY = yAVG->avg();

    if (avgX < 1 || avgY < 1)
    {
        return;
    }


    handShift_X = avgX - previousActiveHandPos.X;
    handShift_Y = avgY - previousActiveHandPos.Y;

    int kinect_x = (int) (avgX * ratioH);
    int kinect_y = (int) (avgY * ratioV);

    if (currentActiveBody.proj_joints[QKinect::LeftHand].Y < currentActiveBody.proj_joints[QKinect::RightHand].Y)
    {
        qDebug() << "Right Hand";
       kinect_x += RH_V_Off;
        kinect_y += RH_H_Off;
    }
    else
      {
          qDebug() << "Left Hand";

          kinect_x += LH_V_Off;
           kinect_y += LH_H_Off;
       }





   qDebug() << "setting POS to X: " << kinect_x << "  Y:  " << kinect_y;
    QCursor::setPos(kinect_x, kinect_y);


    // hand has moved to a different spot -- reset timer
    if ((handShift_X > waverThreshold_X) ||
	(handShift_Y > waverThreshold_Y))
      {
        lastTime = kinectTime;
      }
    else 
      {
        // hand is in same spot for period -- simulate click
        if ((kinectTime - lastTime)>MOUSE_CLICK_HOVER_PERIOD)
	  {

    if (! (display = XOpenDisplay(NULL)))
              {
                  qDebug() << "can not open display";
              }
              XTestFakeButtonEvent(display, 1,TRUE, 0);
              XTestFakeButtonEvent(display,1,FALSE,0);
            XCloseDisplay(display);
	      lastTime = kinectTime;


       }
      }
    return;
}


/*****************************
 *  METHOD: MainWindow::closeEvent(QCloseEvent *event)
 *  PRE:  Called when the window is closed
 *  POST:  tabs are told to save history
 ********************************/

void MainWindow::closeEvent(QCloseEvent * e)
{
    qDebug() << "just sent a close event";
    ((BrowserTab*) tabs->widget(0))->saveHistory();
}


/**************************
  *
  *  Tab control functions
  *  The following functions are used for sending
  *    signals to and from the Tab widget
  *
  ************************/

/*****************************
 *  METHOD: MainWindow::addNewTabSlot()
 *  PRE:  user clicks on corner tab button
 *  POST:  opens up a new tab and switches to it
 ********************************/

void MainWindow::addNewTabSlot(){
    tabs->addTab(new BrowserTab(tabs), "");
    tabs->setCurrentIndex(tabs->count()-1);
    connect(((BrowserTab*)tabs->currentWidget()),
            SIGNAL(lineEditClicked()),
            this,
            SLOT(showKeyboardSlot()));
}



/*****************************
 *  METHOD: MainWindow::addHistoryTabSlot()
 *  PRE:  user clicks on history button
 *  POST:  adds a new history tab.  changes title of tab to "History"
 ********************************/

void MainWindow::addHistoryTabSlot()
{
    tabs->addTab(new BrowserTab(tabs, true),"");
    qDebug() << "tabs count: " << tabs->count();
    tabs->setCurrentIndex(tabs->count()-1);

    ((BrowserTab *) tabs->widget(tabs->count()-1))->getBrowser()->changeTitle("History");
}

/*****************************
 *  METHOD: MainWindow::connectBrowserToTabTitleSlot (int index)
 *  PRE:  a tab is added to the tab bar
 *  POST:  The browser is connected to the tab's title
 ********************************/

void MainWindow::connectBrowserToTabTitleSlot(int index)
{
    connect(((BrowserTab *) tabs->widget(index))->getBrowser(),
            SIGNAL(titleChanged(QString)), this, SLOT(setTabTitleSlot(QString)), Qt::UniqueConnection);


    connect(((BrowserTab *) tabs->widget(index))->getHistoryButton(),
            SIGNAL(clicked()),
            this,
            SLOT(addHistoryTabSlot()), Qt::UniqueConnection);



    connect(((BrowserTab *) tabs->widget(index)),
            SIGNAL(historyClicked()),
            this,
            SLOT(addHistoryTabSlot()), Qt::UniqueConnection);

}


/*****************************
 *  METHOD: MainWindow::setTabTitleSlot(Qstring title)
 *  PRE:  Called when the title of browser changes
 *  POST:  title of tab is changed
 ********************************/

void MainWindow::setTabTitleSlot(QString title)
{
  tabs->setTabText(tabs->currentIndex(),(title.append("            ")).left(12));
}

/*****************************
 *  METHOD: MainWindow::closeTabSlot(int index)
 *  PRE:  user clicks close button on tab
 *  POST:  tab closes.  if it is the only tab, a new one opens
 ********************************/

void MainWindow::closeTabSlot(int index)
{
    if ((index || (tabs->count() > 1)))
    {
        tabs->removeTab(index);
    }
    else
    {
        addNewTabSlot();
        tabs->removeTab(0);

    }

}



MainWindow::~MainWindow() {
}










