#include "qblankwebpage.h"

#include <QDebug>
#include <QNetworkRequest>

QBlankWebPage::QBlankWebPage()
{

}

QWebPage *QBlankWebPage::createWindow(QWebPage::WebWindowType)
{
    return this;
}

bool QBlankWebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type)
{
    if( request.url().path().endsWith(".pdf", Qt::CaseInsensitive) ) {
        qInfo() << "QBlankWebPage::acceptNavigationRequest got PDF:"
                   "\n    request.url() = " << request.url() <<
                   "\n    request.url().path() = " << request.url().path();
        emit requestForFileDownload(request);
        return false;
    }
    return QWebPage::acceptNavigationRequest(frame, request, type);
}
