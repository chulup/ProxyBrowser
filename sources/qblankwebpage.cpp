#include "qblankwebpage.h"

QBlankWebPage::QBlankWebPage()
{

}

QWebPage *QBlankWebPage::createWindow(QWebPage::WebWindowType type)
{
    return this;
}
