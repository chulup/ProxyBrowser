#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QSharedPointer>

class FileDownloader;
class QNetworkRequest;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class ButtonVisible {
        STOP,
        RELOAD
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void updateButtons(ButtonVisible button);

public slots:
    void loadStarted();
    void loadFinished(bool);
    void openHomePage();
    void openPage();
    void savePage();
    void printPage();
    void downloadFile(const QNetworkRequest &request);

    void fileDownloadStarted(FileDownloader *downloader);
    void fileDownloadFinished(FileDownloader *downloader);

private:
    Ui::MainWindow *ui;
    QList<QSharedPointer<FileDownloader>> _fileDownloaders;
};

#endif // MAINWINDOW_H
