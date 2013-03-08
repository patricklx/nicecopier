#include "mytaskbaricon.h"
#include "nicecopier.h"
#include <QMessageBox>
#include <QApplication>
#include <QBuffer>
#include <QSessionManager>
#include "ncsettings.h"
#include "RecentTasks.h"
#include "tasks_dialog_t.h"


MyTaskBarIcon::MyTaskBarIcon(QWidget *_parent)
    :QSystemTrayIcon(QIcon(":/icons/Copy2.ico"),_parent),menu(_parent)
{
    parent =(NiceCopier*) _parent;
    this->setToolTip("NiceCopier");

    connect(&menu, SIGNAL(triggered(QAction *)),
            this,  SLOT(menuEvent(QAction * )));

    connect(this,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),SLOT(activated(QSystemTrayIcon::ActivationReason)));
    connect(this,SIGNAL(messageClicked()),SLOT(askDownload()));


    menu.addAction(tr("Exit"));
    menu.addSeparator();
    menu.addAction(tr("About"));
    //menu.addAction("download new version");
    menu.addAction(tr("recent tasks"));
    menu.addAction(tr("ShutDown: off"))->setToolTip(tr("ShutDown windows after last task"));
    menu.addAction(tr("Settings"));
    setContextMenu(&menu);

    show();
    QTimer::singleShot(60*1000,this,SLOT(resetTaskBarIcon()));
}

void MyTaskBarIcon::resetTaskBarIcon()
{
    hide();
    show();
}


void MyTaskBarIcon::menuEvent(QAction * act)
{
    if( act->text() == tr("Exit") )
    {
        NcSettings::setIsExiting(true);
        QApplication::exit();
        return;
    }

    if( act->text() == tr("Settings") )
    {
        NcSettings *dlgsettings = new NcSettings(parent);
        dlgsettings->setAttribute(Qt::WA_DeleteOnClose);
        connect(dlgsettings,SIGNAL(settings_changed()),parent,SLOT(settingsChanged()));
        dlgsettings->show();
        return;
    }

    if( act->text() == tr("ShutDown: off") )
    {
        if ( !NcSettings::isRunningAsAdmin() || !NcSettings::getShutDownRights() )
        {
            QMessageBox::information(NULL,tr("ShutDown"),tr("You must be admin to enable this option"));
            return;
        }
        int ans = QMessageBox::information(NULL,tr("ShutDown"),
                                           tr("Are you sure you want to shutdown Windows after the last copy finished?"),
                                           QMessageBox::Yes|QMessageBox::No);
        if(ans==QMessageBox::Yes)
        {
            act->setText(tr("ShutDown: on"));
            NcSettings::setShutDown(true);
        }
        return;
    }

    if( act->text() == tr("recent tasks") )
    {
        TasksDialog dialog(parent);
        dialog.move(NcSettings::screenCenter()-dialog.rect().bottomRight()/2);
        dialog.exec();
        if(!dialog.choosen.isEmpty())
        {
            QString taskname = dialog.choosen;
            QString content = RecentTasks::getTask(taskname);
            qDebug()<<"MyTaskBarIcon: "<<"execute: %s";
            QByteArray array = content.toUtf8();
            QBuffer buffer(&array);
            buffer.open(QIODevice::ReadOnly);
            parent->addTask(buffer,false);
        }
        return;
    }

    if( act->text() == tr("ShutDown: on") )
    {
        act->setText(tr("ShutDown: off"));
        NcSettings::setShutDown(false);
        return;
    }

    if( act->text() == tr("About") )
    {
        QMessageBox aboutmsg;
        aboutmsg.setIconPixmap(QPixmap(":icons/Copy2.ico"));
        QString about;
        qDebug()<<"MyTaskBarIcon: build date: "<<__DATE__;
        QDate buildDate = QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));
        about = tr("<h3>Nicopier - %1</h3>\n").arg(buildDate.toString("yy.MM.dd")) +
                tr("NiceCopier is a free program. We are grateful\n to SourceForge.net for our project hosting.\nThis Program is available for Windows XP and later.")+
                +"<br><a href=\"http://sourceforge.net/projects/nicecopier\">http://sourceforge.net/projects/nicecopier/</a><br>"+
                tr("<h3>Licence:</h3>")+
                "GPL"+
                tr("<h3>Icons:</h3>")+
                "<a href=\"www.iconfinder.com\">www.iconfinder.com</a><br>"+
                "<a href=\"www.vistaico.com\">www.vistaico.com</a>"
                ;
        aboutmsg.setText(about);
        aboutmsg.exec();
        return;
    }

    if( act->text() == tr("download new version") )
    {
        emit getUpdate();
        return;
    }
}

void MyTaskBarIcon::newVersionAvailable()
{
    showMessage(tr("New Version available"),
                tr("A newer version of NiceCopier is available."));

    menu.clear();
    menu.addAction(tr("Exit"));
    menu.addSeparator();
    menu.addAction(tr("About"));
    menu.addAction(tr("download new version"));
    menu.addAction(tr("recent tasks"));
    menu.addAction(tr("ShutDown: off"))->setToolTip(tr("ShutDown windows after last task"));
    menu.addAction(tr("Settings"));

    newerVersionAvailable = true;
}

void MyTaskBarIcon::activated(QSystemTrayIcon::ActivationReason reason)
{
    menu.setWindowOpacity(1.0);
}

void MyTaskBarIcon::showMessage(const QString &title, const QString &msg, MessageIcon icon, int msecs)
{
    lastMessage = msg;
    QSystemTrayIcon::showMessage(title,msg,icon,msecs);
}

void MyTaskBarIcon::askDownload()
{
    if(lastMessage == tr("A newer version of NiceCopier is available."))
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Update"));
        msg.setText(tr("Update NiceCopier now?"));
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        msg.show();
        msg.move(NcSettings::screenCenter()-msg.rect().bottomRight()/2);
        int ans = msg.exec();
        if(ans==QMessageBox::Yes)
        {
            emit getUpdate();
        }
    }
}
