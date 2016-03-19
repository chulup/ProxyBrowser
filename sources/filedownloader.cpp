#include "filedownloader.h"

#include <QStandardPaths>

#include "mainwindow.h"

FileDownloader::FileDownloader(MainWindow *window, const QNetworkRequest &request) :
    QObject(window),
    _request(request),
    _window(window)
{
    // Start the download process immediately, rather than waiting for a call
    beginDownload();
}

bool FileDownloader::downloadFinishedSuccessfully() const
{
    return _downloadError.isEmpty();
}

void FileDownloader::beginDownload()
{
    // Make our strings for the File
    auto downloadFileName = QFileInfo(_request.url().path()).fileName();
    qDebug() << "Downloading file with file name: " << downloadFileName;

    auto absoluteFileAddress = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
            + "/" + downloadFileName; // e.g. file name appended to the directory
    qDebug() << "Downloaded file will be stored at: " << absoluteFileAddress;

    _file.reset(new QFile(absoluteFileAddress));
    qDebug() << "File being downloaded to: " + absoluteFileAddress;

    // Check if we have the file open for writing
    if (!(_file->open(QIODevice::WriteOnly)))
    {
        _downloadError = _file->errorString();
        _window->fileDownloadFinished(this);
        return; // Break, failed
    }

    // Get the data from our request on the URL
    _networkReply.reset(_networkManager.get(_request));
    _window->fileDownloadStarted(this);

    // Setup signals/slots
    connect(_networkReply.data(), &QNetworkReply::finished, this, &FileDownloader::downloadFinished);
    connect(_networkReply.data(), &QNetworkReply::readyRead, this, &FileDownloader::downloadReadyRead);
}

void FileDownloader::cancelDownload()
{
    _networkReply->abort();
    _downloadError = "download cancelled";
}

void FileDownloader::downloadFinished()
{
    // Make sure everything is written before we continue
    downloadReadyRead();
    _file->flush();
    _file->close();

    // If there was an error, emit it so we can handle it and/or log it
    if (_networkReply->error())
    {
        qInfo() << "File download finished with error."
                << "\n   URL  : " << _request.url()
                << "\n   Error: " << _networkReply->errorString();
        _downloadError = _file->errorString();
    }

    _networkReply->deleteLater(); // Very important that we use deleteLater()!
    _networkReply.reset();

    _file.reset();

    // Tell the application we are done
    _window->fileDownloadFinished(this);
}

void FileDownloader::downloadReadyRead()
{
    // If pFile exists, dump all reply data to disk to save memory
    if (_file)
    {
        _file->write(_networkReply->readAll());
    }
}
