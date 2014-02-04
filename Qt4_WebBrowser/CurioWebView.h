/* Filename: CurioWebView.h
 * 
 * Objects:  (1) CurioWebView extends QWebView
 *            
 *  The CurioWebView contains the browser 
 *
 */


#ifndef CURIOWEBVIEW_H
#define CURIOWEBVIEW_H


#include <QtWebKit/QtWebKit>
#include <QMenu>
#include <QUrl>
#include "ModalKeyboard.h"

class CurioWebView : public QWebView
{
    Q_OBJECT
public:
    explicit CurioWebView(QWidget *parent = 0, ModalKeyboard * = 0);
     ~CurioWebView();
    void changeTitle(const QString &);
    void handleKinectMouse(int, int);


signals:
    void webPageVisited(const QUrl &, const QString &);

public slots:
    void urlChangedSlot(bool);

protected:
    void contextMenuEvent ( QContextMenuEvent * ev );

private :

    ModalKeyboard * tabKeyboard;

};

#endif // CURIOWEBVIEW_H
