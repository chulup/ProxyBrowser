#ifndef QBLANKWEBPAGE_H
#define QBLANKWEBPAGE_H

#include <QWebPage>

class QBlankWebPage : public QWebPage
{
public:
    QBlankWebPage();

    virtual QWebPage *createWindow(WebWindowType type) override;
    virtual bool acceptNavigationRequest(QWebFrame *frame,
                                         const QNetworkRequest &request,
                                         NavigationType type) override;
Q_SIGNALS:
    void requestForFileDownload(const QNetworkRequest &);

};

#endif // QBLANKWEBPAGE_H
