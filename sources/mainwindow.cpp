#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLineEdit>

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
