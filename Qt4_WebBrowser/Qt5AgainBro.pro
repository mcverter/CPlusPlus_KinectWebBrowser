#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T17:18:31
#
#-------------------------------------------------

QT       += core gui webkitwidgets quick  qml network dbus testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt5Browser
TEMPLATE = app

LIBS += -ludev   -lX11 -lXtst
CONFIG += debug


SOURCES += \
    History.cpp \
    CurioWebView.cpp \
    CompleterLineEdit.cpp \
    BrowserTab.cpp \
    QKinectWrapper.cpp \
    NavigationBar.cpp \
    ModalKeyboard.cpp \
    MainWindow.cpp \
    main.cpp \
    SMA.cpp


HEADERS  += \
    History.h \
    CurioWebView.h \
    CompleterLineEdit.h \
    BrowserTab.h \
    QKinectWrapper.h \
    NavigationBar.h \
    ModalKeyboard.h \
    MainWindow.h \
    SMA.h


OTHER_FILES += \
    history.txt \
    icons/reload.png \
    icons/home.ico \
    icons/history.png \
    icons/go.png \
    icons/forward.png \
    icons/backward.png \
    icons/addTab.png \
    qml/VirtualKeyboardKey.qml \
    qml/VirtualKeyboard.qml \
    qml/main.qml \
    ../../Downloads/KinectRapper/example/KinectReader/logo3.png \
    ../../Downloads/KinectRapper/example/KinectReader/KinectReader.pro.user.85c52c5.2.7pre1 \
    ../../Downloads/KinectRapper/example/KinectReader/KinectReader.pro.user \
    ../../Downloads/KinectRapper/example/KinectReader/KinectReader.pro



QTKINECTWRAPPER_SOURCE_PATH = /home/dsion/Downloads/KinectRapper/QtKinectWrapper

OPENNI_LIBRARY_PATH = /usr/include/ni
#USB_CONTROLLER_INCLUDE_PATH = /home/dsion/Downloads/qt-qtsystems/src/systeminfo


#INCLUDEPATH += $$QTKINECTWRAPPER_SOURCE_PATH
INCLUDEPATH += USB_CONTROLLER_INCLUDE_PATH

INCLUDEPATH += $$QTKINECTWRAPPER_SOURCE_PATH/OpenNI/Include

   # Iterate through
   for(l, OPENNI_LIBRARY_PATH):LIBS+=-L$$l/lib
 LIBS += -L$$OPENNI_LIBRARY_PATH/lib
INCLUDEPATH += /usr/include/ni

LIBS += -L/usr/lib -lOpenNI

RESOURCES += \
    ../../Downloads/KinectRapper/example/KinectReader/resources.qrc

FORMS += \
    ../../Downloads/KinectRapper/example/KinectReader/mainwindow.ui

