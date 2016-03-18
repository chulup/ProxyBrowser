#include "downloadfilehandler.h"

DownloadFileHandler::DownloadFileHandler(const QUrl &downloadUrl, const QDir &storageLocation, bool startImmediately)
    : QObject(), mDownloadUrl(downloadUrl), mStorageLocation(storageLocation)
{
    // Start the download process immediately, rather than waiting for a call
    if (startImmediately)
        beginDownload();
}

void DownloadFileHandler::beginDownload()
{
    // Make our strings for the File
    QString downloadFileName = QFileInfo(mDownloadUrl.path()).fileName();
    qDebug() << "Downloading file with file name: " << downloadFileName;

    QString absoluteFileAddress = mStorageLocation.path() + "/" + downloadFileName; // e.g. file name appended to the directory
    qDebug() << "Downloaded file will be stored at: " << absoluteFileAddress;

    pFile = new QFile(absoluteFileAddress);
    qDebug() << "File being downloaded to: " + absoluteFileAddress;

    // Check if we have the file open for writing
    if (!pFile->open(QIODevice::WriteOnly))
    {
        // Send our message to the application
        emit fileOpeningError(pFile->errorString());

        // Delete file
        if (pFile)
        {
            delete pFile;
            pFile = NULL;
        }

        return; // Break, failed
    }

    // Get the data from our request on the URL
    pNetworkReply = mNetworkManager.get(QNetworkRequest(mDownloadUrl));
    emit fileDownloadStarted();

    // Setup signals/slots
    connect(pNetworkReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(pNetworkReply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
}

void DownloadFileHandler::cancelDownload()
{
    pNetworkReply->abort();
    emit fileDownloadCancelled();
}

void DownloadFileHandler::downloadFinished()
{
    // Make sure everything is written before we continue
    downloadReadyRead();
    pFile->flush();
    pFile->close();

    // If there was an error, emit it so we can handle it and/or log it
    if (pNetworkReply->error())
    {
        emit fileDownloadError(pNetworkReply->errorString());
    }

    pNetworkReply->deleteLater(); // Very important that we use deleteLater()!
    pNetworkReply = NULL;

    delete pFile;
    pFile = NULL;

    // Tell the application we are done
    emit fileDownloadFinished();
}

void DownloadFileHandler::downloadReadyRead()
{
    // If pFile exists, dump all reply data to disk to save memory
    if (pFile)
    {
        pFile->write(pNetworkReply->readAll());
    }
}

void DownloadFileHandler::setDownloadURLSlot(const QString &url)
{
    this->setDownloadURL(QUrl(url));
}
