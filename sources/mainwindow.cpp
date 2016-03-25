#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QErrorMessage>
#include <QFileDialog>
#include <QLineEdit>
#include <QLoggingCategory>
#include <QNetworkProxy>
#include <QtDebug>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QSettings>
#include <QWebElementCollection>
#include <QWebFrame>
#include <QWebHistory>

#include "qblankwebpage.h"
#include "constants.h"
#include "filedownloader.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName(CNAME);
    QCoreApplication::setApplicationName(PNAME);

    ui->centralWidget->layout()->removeWidget(ui->addressLine);
    ui->mainToolBar->insertWidget(ui->actionGo, ui->addressLine);

    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false");

    auto page = new QBlankWebPage();
    ui->webView->setPage(page);
    ui->webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    ui->webView->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);

    connect(page, &QBlankWebPage::requestForFileDownload,
            this, &MainWindow::downloadFile);
    connect(&_timer, &QTimer::timeout, this, &MainWindow::loadTimeout);

    rereadSettings();

    openHomePage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateButtons(MainWindow::ButtonVisible button)
{
    ui->actionReload->setVisible(button == ButtonVisible::RELOAD);
    ui->actionStop->setVisible(button == ButtonVisible::STOP);
    if ( !ui->webView->url().isEmpty() ){
        ui->addressLine->setText(ui->webView->url().toString());
    }
    ui->actionBackward->setEnabled(ui->webView->history()->canGoBack());
    ui->actionForward->setEnabled(ui->webView->history()->canGoForward());
}

void MainWindow::loadStarted() {
    qInfo() << "loadStarted";
    updateButtons(ButtonVisible::STOP);
    _timer.start();
}

void MainWindow::loadTimeout()
{
    qInfo() << "loadTimeout, bytes: " << ui->webView->page()->bytesReceived();
    if( ui->webView->page()->bytesReceived() < 94 /* chosen by fair dice roll */ ) {
        ui->webView->stop();
        ui->webView->setHtml("<h1> Connection timeout </h1>");
    }
}

void MainWindow::loadFinished(bool ok){
    qInfo() << "loadFinished(" << ok << ")";
    updateButtons(ButtonVisible::RELOAD);
    if (!ok) {
        setWindowTitle("Load failed: " +  ui->addressLine->text());
        ui->webView->setHtml("<h1> Connection timeout </h1>");
    }
    _timer.stop();
}

void MainWindow::titleChanged(const QString &title)
{
    setWindowTitle(title);
}

void MainWindow::openHomePage()
{
    qInfo() << "loadStarted";
    QSettings settings;
    ui->addressLine->setText(settings.value(SET_HOME_PAGE, QVariant(HOME_PAGE)).toString());
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
    auto pagename = ui->webView->title().toHtmlEscaped();
    auto filename = QFileDialog::getSaveFileName(this,
                                                 "Save page as...",
                                                 QString(),
                                                 "PDF (*.pdf)",
                                                 &pagename);
    if(filename.isEmpty()) {
        qInfo() << "    No file selected.";
        return;
    }
    QPrinter printer;
    printer.setOutputFileName(filename);
    printer.setOutputFormat(QPrinter::PdfFormat);
    ui->webView->print(&printer);
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

void MainWindow::downloadFile(const QNetworkRequest &request)
{
    _fileDownloaders.push_back(QSharedPointer<FileDownloader>::create(this, request));
}

void MainWindow::fileDownloadStarted(FileDownloader *downloader)
{
    qInfo() << "MainWindow::fileDownloadStarted for url " << downloader->url();
}

void MainWindow::fileDownloadFinished(FileDownloader *downloader)
{
    if (downloader->downloadFinishedSuccessfully() ) {
        qInfo() << "MainWindow::fileDownloadFinished successfully. Opening the file...";
        if( !QDesktopServices::openUrl(QUrl::fromLocalFile(downloader->filename())) ) {
            QErrorMessage::qtHandler()->showMessage(
                        "File was downloaded successfully but could not be opened automatically. "
                        "You may open it manually.\n"
                        "File name: " + downloader->filename());
        }
    } else {
        qInfo() << "MainWindow::fileDownloadFinished with error: " << downloader->downloadError();
    }
    std::remove_if(std::begin(_fileDownloaders),
                   std::end(_fileDownloaders),
                   [downloader](QSharedPointer<FileDownloader> pointer)
                    {
                        return pointer == downloader;
    });
}

void MainWindow::rereadSettings()
{
    QSettings settings;
    if (settings.value(SET_PROXY_ENABLED, QVariant(PROXY_ENABLED)).toBool()) {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(settings.value(SET_PROXY_HOST, QVariant(PROXY_HOST)).toString());
        proxy.setPort(settings.value(SET_PROXY_PORT, QVariant(PROXY_PORT)).toInt());
        proxy.setUser(settings.value(SET_PROXY_USER, QVariant(PROXY_USER)).toString());
        proxy.setPassword(settings.value(SET_PROXY_PASS, QVariant(PROXY_PASS)).toString());
        QNetworkProxy::setApplicationProxy(proxy);
    }

    _timer.setSingleShot(true);
    _timer.setInterval(settings.value(SET_TIMEOUT, QVariant(CONNECT_TIMEOUT)).toInt());
}

void MainWindow::showSettings()
{
    Settings dlg(this);
    dlg.show();
    if(dlg.exec() == QDialog::Accepted) {
        rereadSettings();
    }
}
