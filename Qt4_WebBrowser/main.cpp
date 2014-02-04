/* Filename: main.cpp
 *
 *  Main Driver for Web Browser component
 *
 *  NOTE:  libflashplayer.so must be installed to enable flash multimedia
 *         Other .so files would be needed for other multimedia
 */

#include <QApplication>
#include "MainWindow.h"
#include <phonon/mediaobject.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("WebBrowser");
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}


