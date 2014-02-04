/* Filename: History.h
 *
 * Objects:  (1) History
 *
 *  The History object stores the history of the
 *     visited webpages
*
*/


#ifndef HISTORY_H
#define HISTORY_H

#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QStringList>
#include <QFile>
#include <QCompleter>
#include <QStringListModel>
#include <QTextStream>



class History : public QObject
{
    Q_OBJECT

public:
    History();
    void saveToFile();
    QString outputHTML();

signals:
    void historyUpdatedSignal(const QStringList&);

public slots:
    void addToHistorySlot(const QUrl &, const QString&);

private:
    void initializeHistoryFromFile();
    static const int MAX_SIZE = 100;
    QString HISTORY_FILENAME;


    int count;
    QStringList titles;
    QStringList urls;
    QList<QDateTime> times;

};



#endif // HISTORY_H
