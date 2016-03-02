#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralWidget->layout()->removeWidget(ui->addressLine);
    ui->mainToolBar->insertWidget(ui->actionGo, ui->addressLine);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadStarted() {
    qInfo() << "loadStarted";
    ui->actionReload->setVisible(false);
    ui->actionStop->setVisible(true);
}

void MainWindow::loadFinished(bool result){
    qInfo() << "loadFinished(" << result << ")";
    ui->actionReload->setVisible(true);
    ui->actionStop->setVisible(false);
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
