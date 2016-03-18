#ifndef QBLANKWEBPAGE_H
#define QBLANKWEBPAGE_H

#include <QWebPage>

class QBlankWebPage : public QWebPage
{
public:
    QBlankWebPage();

    virtual QWebPage *createWindow(WebWindowType type) override;
};

#endif // QBLANKWEBPAGE_H
