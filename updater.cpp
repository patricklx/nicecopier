#include "updater.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QRegExp>
#include <QDesktopServices>
#include <QDate>
#include <QLocale>
#include <QDebug>
#include <QApplication>

#include "ncsettings.h"
#include <QFile>
#include <QVariant>
#include <QDir>
#include <QProcess>
#include <QElapsedTimer>
#include <QMessageBox>
#include "TaskThread/ReadableSize.h"

Updater::Updater()
{
    progressDlg.setAttribute(Qt::WA_DeleteOnClose,false);
    progressDlg.setLabelText(tr("Downloading NiceCopier Setup..."));
    niceCopierSetup_File = NULL;

    QFile::remove("NiceCopier_Setup.exe");
    inform = false;
}


void Updater::check(bool inform_user)
{
    QUrl url = QUrl("http://appdriverupdate.sourceforge.net/Files/Updates/NiceCopier.txt");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(check_version()));

    inform = inform_user;
}


void Updater::check_version()
{
    QString html = reply->readAll();
    reply->deleteLater();
    reply = NULL;

    QRegExp regex("Version< ([^ ]*) >");
    regex.setMinimal(true);

    regex.indexIn(html);
    QString version = regex.cap(1);

    if(version.isEmpty())
    {
        qDebug("Updater: no version found: %s",regex.errorString().toUtf8().data());
        if(inform)
            QMessageBox::information(NULL,
                                     tr("Version check"),
                                     tr("No new version found"));
        return;
    }

    QDate buildDate = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    QString act_version = buildDate.toString("yy.MM.dd");
    qDebug("Updater: '%s'",act_version.toUtf8().data());
    latest_version = version;
    if(version == act_version)
    {
        qDebug("Updater: same version found");
        if(inform)
            QMessageBox::information(NULL,
                                     tr("Version check"),
                                     tr("No new version found"));
        return;
    }

    emit newerVersionAvailable();
}

QUrl redirectUrl(const QUrl& possibleRedirectUrl, const QUrl& oldRedirectUrl)
{
    QUrl redirectUrl;
    if(!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != oldRedirectUrl)
        redirectUrl = possibleRedirectUrl;
    return redirectUrl;
}

void Updater::showProgress()
{
    niceCopierSetup_File = new QFile("NiceCopier_Setup.exe");
    if(!niceCopierSetup_File->open(QFile::WriteOnly))
    {
        qDebug("Updater: can't open file");
        progressDlg.close();
        progressDlg.hide();
        reply->abort();
        reply->deleteLater();
        reply = NULL;
        return;
    }

    //connect(reply,SIGNAL(finished()),&progressDlg,SLOT(accept()));
    connect(reply,SIGNAL(finished()),this,SLOT(downloadFinished()));

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(setProgress(qint64,qint64)));
    connect(&progressDlg,SIGNAL(canceled()),this,SLOT(cancel()));



    progressDlg.show();
    progressDlg.move(NcSettings::screenCenter()-progressDlg.rect().bottomRight()/2);

    timer.start();
}

void Updater::getRedirect()
{
    QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(!possibleRedirectUrl.toString().isEmpty())
    {
        qDebug("Updater: redirect");
        qDebug(possibleRedirectUrl.toString().toUtf8());
        reply->deleteLater();
        reply = qnam.get(QNetworkRequest(possibleRedirectUrl.toString()));
        showProgress();
        return;
    }
    QString html = reply->readAll();
    QRegExp regex("url=([^\"]*)\"");
    regex.indexIn(html);

    QString url = regex.cap(1);
    url.replace("&amp;","&");
    qDebug("Updater: follow");
    qDebug("Updater: "+url.toUtf8());

    reply->deleteLater();
    reply = qnam.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(getRedirect()));
}

void Updater::startDownload()
{
    //get download url
    QUrl url = QUrl("http://appdriverupdate.sourceforge.net/Files/Updates/NiceCopier.txt");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(download()));
}

void Updater::download()
{
    QString html = reply->readAll();
    reply->deleteLater();
    reply = NULL;

    QString regexStr="Link< ([^ ]*) >";
    if(QSysInfo::WordSize == 64)
        regexStr = "Link64< ([^ ]*) >";

    QRegExp regex(regexStr);
    regex.setMinimal(true);

    regex.indexIn(html);
    QString link = regex.cap(1);

    if(link.isEmpty())
    {
        qDebug("Updater: no url link found: %s",regex.errorString().toUtf8().data());
        return;
    }
    qDebug() << "Updater: link:"<<link;

    QUrl url = QUrl(link);
    reply = qnam.get(QNetworkRequest(url));

    connect(reply,SIGNAL(finished()),this,SLOT(getRedirect()));
}

void Updater::downloadFinished()
{
    qDebug("Updater: downloaded");

    QProcess proc;
    QStringList args;
    QString launchCommand = QDir::currentPath()+"/NiceCopier_Setup.exe";

    if(niceCopierSetup_File==NULL)
        return;

    niceCopierSetup_File->close();

    if(reply == NULL)
        return;
    reply->deleteLater();
    reply = NULL;


    niceCopierSetup_File->deleteLater();
    niceCopierSetup_File = NULL;

    progressDlg.close();
    progressDlg.hide();

    args << "/c" << "start" << """" << launchCommand;
    proc.startDetached("cmd",args);
    proc.waitForStarted();
    QApplication::exit();

}

void Updater::cancel()
{
    qDebug("Updater: abort");
    if(reply == NULL)
        return;

    if(niceCopierSetup_File==NULL)
        return;

    niceCopierSetup_File->deleteLater();
    niceCopierSetup_File = NULL;
    reply->disconnect();
    reply->abort();
    reply->deleteLater();
    reply=NULL;
    progressDlg.close();
}

void Updater::setProgress(qint64 recieved, qint64 total)
{
    if(reply==NULL)
    {
        qDebug("Updater: reply is NULL??");
        return;
    }

    QByteArray array = reply->readAll();
    niceCopierSetup_File->write(array,array.count());

    int time = timer.elapsed();

    QString speed = Util::toReadableSpeed(recieved/time*1000);
    QString done = Util::toReadableSize(recieved);
    QString size = Util::toReadableSize(total);

    progressDlg.setLabelText(tr("Downloading NiceCopier Setup... [ %1 of %2 ] ( %3 )").arg(done).arg(size).arg(speed));

    progressDlg.setRange(0,total);
    progressDlg.setValue(recieved);

}





