#ifndef QBLANKWEBPAGE_H
#define QBLANKWEBPAGE_H

#include <QWebPage>

class QBlankWebPage : public QWebPage
{
    Q_OBJECT
public:
    QBlankWebPage();

    virtual QWebPage *createWindow(WebWindowType type) override;
    virtual bool acceptNavigationRequest(QWebFrame *frame,
                                         const QNetworkRequest &request,
                                         NavigationType type) override;
Q_SIGNALS:
    void requestForFileDownload(const QNetworkRequest &request);

};

#endif // QBLANKWEBPAGE_H
