#ifndef DOWNLOADFILEHANDLER_H
#define DOWNLOADFILEHANDLER_H

#include <QScopedPointer>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class MainWindow;

class FileDownloader : public QObject
{
    Q_OBJECT

public:
    FileDownloader(MainWindow *window, const QNetworkRequest &request);

    bool downloadFinishedSuccessfully() const;
    const QString &downloadError() const { return _downloadError; }
    QString filename() const;
    QUrl url() const;

Q_SIGNALS:
    void downloadStarted(FileDownloader *downloader);
    void downloadFinished(FileDownloader *downloader);

public Q_SLOTS:
    void beginDownload();
    void cancelDownload();
    void downloadFinished();
    void downloadReadyRead();

private:
    QScopedPointer<QFile> _file;

    QNetworkRequest _request;
    QScopedPointer<QNetworkReply> _networkReply;
    QNetworkAccessManager _networkManager;
    MainWindow *_window;

    QString _downloadError;
};

#endif // DOWNLOADFILEHANDLER_H
