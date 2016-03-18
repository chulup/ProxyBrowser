#ifndef DOWNLOADFILEHANDLER_H
#define DOWNLOADFILEHANDLER_H

#include <QDir>
#include <QFile>
#include <QNetworkAccessManager>
#include <QUrl>

class DownloadFileHandler : public QObject
{
    Q_OBJECT

public:
    DownloadFileHandler(const QUrl &downloadUrl, const QDir &storageLocation,
                        bool startImmediately = false);

    DownloadFileHandler() : QObject() {}

    inline void setFile(QFile *file) { pFile = file; }
    inline void setStorageLocation(const QDir &location) { mStorageLocation = location; }
    inline void setDownloadURL(const QUrl &url) { mDownloadUrl = url; }

    inline QFile* file() const { return pFile; }
    inline QDir storageLocation() const { return mStorageLocation; }
    inline QUrl downloadUrl() const { return mDownloadUrl; }

signals:
    void fileAlreadyExistsError(QString existingFileDisplayName);
    void fileOpeningError(QString errorMessage);
    void fileDownloadError(QString errorMessage);
    void fileDownloadStarted();
    void fileDownloadCancelled();
    void fileDownloadFinished();

public slots:
    void setDownloadURLSlot(const QString &url);

    void beginDownload();
    void cancelDownload();
    void downloadFinished();
    void downloadReadyRead();

private:
    QFile *pFile;
    QDir mStorageLocation;

    QUrl mDownloadUrl;

    QNetworkReply *pNetworkReply;
    QNetworkAccessManager mNetworkManager;
};

#endif // DOWNLOADFILEHANDLER_H
