#include "updater_t.h"

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QRegExp>
#include <QDesktopServices>
#include <QDate>
#include <QLocale>

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
    progressDlg.setLabelText("Downloading NiceCopier Setup...");
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
        qDebug("no version found: %s",regex.errorString().toAscii().data());
        if(inform)
            QMessageBox::information(NULL,"Version check","No new version found");
        return;
    }

    QDate buildDate = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
    QString act_version = buildDate.toString("yy.MM.dd");
    qDebug("'%s'",act_version.toAscii().data());
    latest_version = version;
    if(version == act_version)
    {
        qDebug("same version found");
        if(inform)
            QMessageBox::information(NULL,"Version check","No new version found");
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
        qDebug("can't open file");
        progressDlg.close();
        progressDlg.hide();
        reply->abort();
        reply->deleteLater();
        reply = NULL;
        return;
    }

    //connect(reply,SIGNAL(finished()),&progressDlg,SLOT(accept()));
    connect(reply,SIGNAL(finished()),this,SLOT(downloaded()));

    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(setProgress(qint64,qint64)));
    connect(&progressDlg,SIGNAL(canceled()),this,SLOT(cancel()));


    progressDlg.move(NcSettings::screenCenter()-progressDlg.rect().bottomRight()/2);
    progressDlg.show();

    timer.start();
}

void Updater::getRedirect()
{
    QVariant possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(!possibleRedirectUrl.toString().isEmpty())
    {
        qDebug("redirect");
        qDebug(possibleRedirectUrl.toString().toAscii());
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
    qDebug("follow");
    qDebug(url.toAscii());

    reply->deleteLater();
    reply = qnam.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(getRedirect()));
}

void Updater::download()
{
    QString link;
    link = "http://sourceforge.net/projects/nicecopier/files/NiceCopier/NiceCopierSetup_{ver}.exe/download";
    if(QSysInfo::WordSize == 64)
        link = "http://sourceforge.net/projects/nicecopier/files/NiceCopier/NiceCopierSetup64_{ver}.exe/download";
    link.replace("{ver}",latest_version);
    qDebug(link.toAscii());
    QUrl url = QUrl(link);
    reply = qnam.get(QNetworkRequest(url));

    connect(reply,SIGNAL(finished()),this,SLOT(getRedirect()));

}

void Updater::downloaded()
{
    qDebug("downloaded");

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

}

void Updater::cancel()
{
    qDebug("abort");
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
        qDebug("reply is NULL??");
        return;
    }

    QByteArray array = reply->readAll();
    niceCopierSetup_File->write(array,array.count());

    int time = timer.elapsed();

    QString speed = Util::toReadableSize(recieved/time*1000);
    QString done = Util::toReadableSize(recieved);
    QString size = Util::toReadableSize(total);

    progressDlg.setLabelText(tr("Downloading NiceCopier Setup... [ %1 of %2 ] ( %3 )").arg(done).arg(size).arg(speed));

    progressDlg.setRange(0,total);
    progressDlg.setValue(recieved);


}

