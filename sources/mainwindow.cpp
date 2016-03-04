#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QtDebug>
#include <QWebHistory>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QNetworkProxy>
#include <QLoggingCategory>

#include "constants.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralWidget->layout()->removeWidget(ui->addressLine);
    ui->mainToolBar->insertWidget(ui->actionGo, ui->addressLine);

    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName(PROXY_HOST);
    proxy.setPort(PROXY_PORT);
    proxy.setUser(PROXY_USER);
    proxy.setPassword(PROXY_PASS);
    QNetworkProxy::setApplicationProxy(proxy);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateButtons(MainWindow::ButtonVisible button)
{
    ui->actionReload->setVisible(button == ButtonVisible::RELOAD);
    ui->actionStop->setVisible(button == ButtonVisible::STOP);
    ui->addressLine->setText(ui->webView->url().toString());
    ui->actionBackward->setEnabled(ui->webView->history()->canGoBack());
    ui->actionForward->setEnabled(ui->webView->history()->canGoForward());
}

void MainWindow::loadStarted() {
    qInfo() << "loadStarted";
    updateButtons(ButtonVisible::STOP);
}

void MainWindow::loadFinished(bool result){
    qInfo() << "loadFinished(" << result << ")";
    updateButtons(ButtonVisible::RELOAD);
}

void MainWindow::openHomePage()
{
    qInfo() << "loadStarted";
    ui->addressLine->setText(HOME_PAGE);
    ui->actionGo->trigger();
}

void MainWindow::openPage(){
    auto rawAddress = ui->addressLine->text();
    if( !rawAddress.contains("://") ) {
        qInfo() << "openPage: adding \"http://\" to address";
        rawAddress.prepend("http://");
        ui->addressLine->setText(rawAddress);
    }
    qInfo() << "openPage with address" << ui->addressLine->text();
    ui->webView->load(QUrl(ui->addressLine->text()));
}

void MainWindow::savePage()
{
    qInfo() << "savePage";
}

void MainWindow::printPage()
{
    qInfo() << "printPage";
    QPrinter printer;
    QPrintPreviewDialog preview(&printer);

    preview.setWindowTitle("Print page \"" + ui->webView->title() + "\"");

    connect(&preview, &QPrintPreviewDialog::paintRequested,
            [&]{
                this->ui->webView->print(&printer);
              });

    preview.exec();
}
