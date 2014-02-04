/* Filename: MainWindow.h
 * 
 * Objects:  (1) MainWindow extends QMainWindow
 *            
 *  The Main Window presents the main interface of the WebBrowser
 *
 * It contains:
 * (a) BrowserTab
 * (b) newTabButton
 * 
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QDateTime>
#include <QMainWindow>
#include <QtWebKit/QtWebKit>
#include <QLayout>
#include <QDebug>
#include <QPushButton>
#include <QDockWidget>
#include "NavigationBar.h"
#include "BrowserTab.h"
#include "History.h"
#include <QCompleter>
#include "BrowserTab.h"
#include "CurioWebView.h"
#include <QKinectWrapper.h>
#include <XnFPSCalculator.h>
#include <QImage>
#include "ModalKeyboard.h"
#include "SMA.h"
#include <QPushButton>
#include <QDockWidget> 
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#undef Bool

class MainWindow : public QMainWindow
 {
    Q_OBJECT
 
public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();


signals:
    void keyboardInputSignal(QString);

   private slots:
    void addNewTabSlot();
    void closeTabSlot(int);
    void connectBrowserToTabTitleSlot(int);
    void setTabTitleSlot(QString title);
    void addHistoryTabSlot();

    void kinectLocationSlot();
    void kinectStatusSlot(QKinect::KinectStatus);
    void kinectUserSlot(unsigned,bool);
    void kinectPoseSlot(unsigned,QString);
    void kinectCalibrationSlot(unsigned,QKinect::CalibrationStatus);

protected:
    void	closeEvent ( QCloseEvent * event );

private:
    QTabWidget * tabs;
    QPushButton * newTabBtn;




    // KINECT

    QKinect::QKinectWrapper kreader;
    XnFPSData xnFPS;

    QImage leftCursor;
    QImage rightCursor;
    QKinect::Body currentActiveBody;

    int active_user_idx;

    XnFloat waverThreshold_X;
    XnFloat waverThreshold_Y;
    qint64 lastTime;
    bool hasLeftArea;



    //For controls
    XnPoint3D activeHandPos;
    XnPoint3D previousActiveHandPos;
    XnFloat refX;
    XnFloat refY;

    XnFloat handShift_X;
    XnFloat handShift_Y;
    XnFloat cursorX;
    XnFloat refX_tmp;
    XnFloat refY_tmp;
    XnFloat cursorY;
    bool mCalibrated;
 Display * display;

    // Amount of time (in microseconds) for
    // mouse hover to produces a click
    static const int MOUSE_CLICK_HOVER_PERIOD = 2000;

    //  for kinect mouse stabilization
    SMA * xAVG;
    SMA * yAVG;
/*
    float screenH;
    float screenV;
    float kinectH;
    float kinectV;
    float ratioH;
    float ratioV;
*/
    static const int ratioH = 5;
    static const int ratioV = 5;
    static const int RH_V_Off = 0;//-500;
    static const int LH_V_Off = 0;//500;
    static const int RH_H_Off = 0;//500;
    static const int LH_H_Off = 0; //500;
    // close button
    QPushButton * closeButton;
    QDockWidget * closeDock;

};

#endif
