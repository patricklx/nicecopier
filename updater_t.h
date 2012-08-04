#ifndef UPDATER_T_H
#define UPDATER_T_H

#include <QObject>
#include <QProgressDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QElapsedTimer>

class Updater:public QObject
{
        Q_OBJECT
        QString latest_version;
        QNetworkAccessManager qnam;
        QProgressDialog progressDlg;
        QNetworkReply *reply;
        QElapsedTimer timer;
        bool inform;
        QFile *niceCopierSetup_File;
    public:
        Updater();


    signals:
        void newerVersionAvailable();

    private slots:
        void downloadFinished();
        void check_version();
        void setProgress(qint64 recieved,qint64 total);
        void cancel();
        void getRedirect();
        void showProgress();

    public slots:
        void check(bool inform_user=false);
        void startDownload();
        void download();


};

#endif // UPDATER_T_H
