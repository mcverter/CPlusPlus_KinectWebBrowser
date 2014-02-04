#include "History.h"


/*****************************
 *  METHOD: History::History()
 *  PRE:  Called when first tab is created
 *  POST:  History object initialized from history.txt file
********************************/

History::History()
{
    HISTORY_FILENAME = "history.txt";
    count = 0;
    initializeHistoryFromFile();

}


/*****************************
 *  METHOD: History::initializeHistoryFromFile()
 *  PRE:  Called from constructor
 *  POST: Opens up history.txt file and parses each line for history item.
********************************/

void History::initializeHistoryFromFile()
{
    QFile history_file(HISTORY_FILENAME);
    if (history_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&history_file);
        while (!in.atEnd())
        {
             QString line = (QString) in.readLine();
             QStringList entry = line.split("######");
             urls.append(entry[0]);
             titles.append(entry[1]);
             times.append(QDateTime::fromString(entry[2],"MMddyyyyhhmm"));
             count++;
        }
    }
    emit historyUpdatedSignal(urls);


}


/*****************************
 *  METHOD: outputHTML()
 *  PRE:  history button clicked
 *  POST:  The html necessary for displaying the history is produced
********************************/

QString History::outputHTML()
{
    QString html = "<!DOCTYPE html>\n\n"
            "<html>\n"
            "<head>\n"
            "<meta charset=\"utf-8\">"
            "<title> History</title>\n"
            "</head>\n"
            "<body>\n"
            "<h2>History</h2>"
            "<ul>";

    for (int i=0; i<count; i++)
    {
        html += "<li><a href=\"" + urls.at(i) + "\">" + titles.at(i) + "</a>  "
                + times.at(i).toString("ddd MMMM d yyyy, hh:mm") + "</li>\n";
    }
    html += "</ul>\n</body>\n</html>";
    return html;

}


/*****************************
 *  METHOD: History::saveToFile()
 *  PRE:  Main Window closed
 *  POST: Browser history is saved to history.txt
********************************/

void History::saveToFile()
{
    QFile history_file(HISTORY_FILENAME);
    history_file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&history_file);

    for (int i=0; i<count; i++)
    {
        out <<   urls.at(i) << "######" << titles.at(i)<< "######"<<
                ((const QDateTime) times.at(i)).toString("MMddyyyyhhmm") <<
                 "\n";
    }
    history_file.close();
}

/*****************************
 *  METHOD: History::addToHistorySlot(QUrl, QString)
 *  PRE:  User visits a new page
 *  POST: The url and string are added to the history lists.
 *      If they are already on the list, the position is updated
********************************/

void History::addToHistorySlot(const QUrl & url, const QString& title)
{
    QString urlAsString = url.toString();
   
    // don't add history tab to history!
    if (urlAsString.compare("about:blank")==0)
    {
        return;
    }

    if (urls.contains(urlAsString))
    {
        int index = urls.indexOf(urlAsString);
        urls.removeAt(index);
        times.removeAt(index);
        titles.removeAt(index);
    }

    else if (count >= MAX_SIZE)
    {
            urls.removeLast();
            times.removeLast();
            titles.removeLast();
     }
    else
    {
        count++;
    }

        urls.prepend(urlAsString);
        times.prepend(QDateTime::currentDateTime());
        titles.prepend(title);


        emit historyUpdatedSignal(urls);

}
