#include "nicecopier.h"
#include "ui_nicecopier.h"

#include "copyinfopanel.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QCloseEvent>
#include <QThread>
#include "ncsettings.h"
#include <QDesktopWidget>
#include <QDir>
#include <QString>
#include <windows.h>
#include <shutdowntimerdialog.h>
#include <QBuffer>
#include <QBitmap>

NiceCopier::NiceCopier(QWidget *parent) :
        QWidget(parent),
    ui(new Ui::NiceCopier)
{
    ui->setupUi(this);

    RecentTasks::initialize();

    NcSettings::load();
    settingsChanged();


    ui->tasklist->setMaxTask(NcSettings::getValue<int>(NcSettings::SHOW_TASKS));


    taskbar = new MyTaskBarIcon(this);

    connect(&server,SIGNAL(newCopyTask(QIODevice&,QLocalSocket*)),SLOT(addTask(QIODevice&)));
    connect(taskbar,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),SLOT(taskIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(QCoreApplication::instance(),SIGNAL(aboutToQuit()),SLOT(aboutToQuit()));
    connect(QApplication::desktop(),SIGNAL(workAreaResized(int)),SLOT(updateWindow()));

    taskhandler = new TaskListHandler;

    QDir dir;
    if( !dir.mkdir("Tasks") )
    {
        QDir checkdir("Tasks");
        if(!checkdir.exists()){
            QString message = QString("Failed to create the directory Tasks\n Start NiceCopier as administrator or")+
                    "create the directory manualy\n otherwise NiceCopier won't save the tasks!";
            QMessageBox::warning(this,
                                 tr("NiceCopier Error"),
                                 tr(message.toUtf8()));
        }
    }

    QTimer::singleShot(1000,this,SLOT(startTasks()));

    connect(&updater,SIGNAL(newerVersionAvailable()),taskbar,SLOT(newVersionAvailable()));
    connect(taskbar,SIGNAL(getUpdate()),&updater,SLOT(startDownload()));
    updateWindow();
}


void NiceCopier::startTasks()
{
    QFile file("Tasks/tasklist.nc");
    if(file.open(QFile::ReadOnly))
    {
        while(!file.atEnd())
        {
            QString task(file.readLine());
            task = task.trimmed();

            QFile file2(task);
            if(file2.open(QFile::ReadOnly))
            {
                addTask(file2,false);
            }
        }
    }
    updateWindow();
}

NiceCopier::~NiceCopier()
{
    NcSettings::save();
    NcSettings::unload();
    while(!panels.isEmpty())
    {
        CopyInfoPanel *p = panels[0];
        panels.removeFirst();
        delete p;
    }
    qDebug("NiceCopier: closing");

    delete ui;
    delete taskbar;
}


void NiceCopier::addTask(QIODevice &info, bool is_new)
{
    CopyInfoPanel *p = new CopyInfoPanel(this,taskhandler);
    connect(p,SIGNAL(finishedEvent(CopyInfoPanel*)),SLOT(removeTask(CopyInfoPanel*)));
    connect(p,SIGNAL(startEvent(CopyInfoPanel*)),SLOT(setFirstPanel(CopyInfoPanel*)));
    connect(p,SIGNAL(expand(CopyInfoPanel*)),SLOT(expandTask(CopyInfoPanel*)));
    connect(p,SIGNAL(contract(CopyInfoPanel*)),SLOT(contractTask(CopyInfoPanel*)));
    connect(p,SIGNAL(createNewTask(QIODevice&)),SLOT(addTask(QIODevice&)));


    QBuffer buffer;
    QIODevice *taskinfo = &info;
    QByteArray array;
    if(is_new)
    {
        array = info.readAll();
        qDebug()<<"NiceCopier: recieved:"<<array;
        buffer.setBuffer(&array);
        buffer.open(QIODevice::ReadOnly);
        taskinfo = &buffer;
    }

    if(!p->createTask(*taskinfo))
    {
        delete p;
        return;
    }

    if(is_new)
        RecentTasks::addRecentTask(array);

    panels.append(p);

    ui->tasklist->addTask(p);

    if(ui->tasklist->count()==1)
        QTimer::singleShot(NcSettings::getValue<int>(NcSettings::TIME_UNTIL_SHOW),this,SLOT(updateWindow()));
    else
        updateWindow();

    QFile file("Tasks/tasklist.nc");
    if(!file.open(QFile::WriteOnly))
        return;


    for(int i=0;i<panels.count();i++)
    {
        CopyInfoPanel *panel = (CopyInfoPanel*) panels.at(i);
        QString info;
        info.sprintf("Tasks/Task%i.Task\n",panel->getTaskId());
        file.write(info.toUtf8());
    }
}

void NiceCopier::removeTask( CopyInfoPanel *panel )
{
    panels.removeOne(panel);
    panel->hide();
    ui->tasklist->removeTask(panel);

    if( !isVisible() && !panel->isStoppedByUser())
    {

        if(!panel->isStoppedByUser())
            taskbar->showMessage(tr("Copy Finished"),panel->getSourceListMsg());
    }
    updateWindow();

    delete panel;

    QFile file("Tasks/tasklist.nc");
    if(!file.open(QFile::WriteOnly))
        return;

    for(int i=0;i<panels.count();i++)
    {
        QString info;
        panel = (CopyInfoPanel*) panels.at(i);
        if(panel->isTaskValid())
        {
            info.sprintf("Tasks/Task%i.Task\n",panel->getTaskId());
            file.write(info.toUtf8());
        }
    }

    if( NcSettings::isShutDown() && panels.count()==0)
    {
        ShutDownTimerDialog d(NULL);
        d.exec();
    }
}

void NiceCopier::expandTask(CopyInfoPanel *panel)
{
    Qt::WindowFlags flags = Qt::WindowStaysOnTopHint|
                            Qt::WindowSystemMenuHint|
                            Qt::CustomizeWindowHint|
                            Qt::Dialog|
                            Qt::WindowMinimizeButtonHint|
                            Qt::WindowMaximizeButtonHint|
                            Qt::WindowCloseButtonHint;
    ui->tasklist->removeTask(panel);
    panel->setParent(NULL,flags);
    panel->show();
    panel->setWindowTitle("NiceCopier Task");
    panel->move(NcSettings::screenCenter()-panel->rect().bottomRight()/2);
    updateWindow();
}

void NiceCopier::contractTask(CopyInfoPanel *panel)
{
    panel->setParent(ui->tasklist->getScrollArea());
    ui->tasklist->insertTask(panel);
    panel->show();
    this->show();
    updateWindow();
}

void NiceCopier::settingsChanged()
{
    int ap = NcSettings::getValue<int>(NcSettings::APPEARANCE);
    Qt::WindowFlags flags = Qt::CustomizeWindowHint;
    if(NcSettings::getValue<bool>(NcSettings::ALWAYS_ON_TOP)){
        flags |= Qt::WindowStaysOnTopHint;
    }
    switch(ap)
    {
    case NcSettings::WIN_TOOL:
        flags = flags|Qt::Tool|Qt::WindowCloseButtonHint;
        break;
    case NcSettings::WIN_DIALOG:
        flags = flags|Qt::Dialog;
        break;
    case NcSettings::WIN_WINDOW:
        flags = flags|Qt::Dialog|Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint;
    }
    this->setWindowFlags(flags);
    this->createWinId();
    qDebug()<<"created: "<<this->testAttribute(Qt::WA_WState_Created);
    ui->tasklist->setMaxTask(NcSettings::getValue<int>(NcSettings::SHOW_TASKS));
    QString title;
    title.sprintf(tr("NiceCopier: Showing %d tasks, %d running").toUtf8(),
                  NcSettings::getValue<int>(NcSettings::SHOW_TASKS),
                  ui->tasklist->count());
    setWindowTitle(title);
    if(NcSettings::getValue<bool>(NcSettings::CHECK_UPDATES))
    {
        QTimer::singleShot(10*1000,&updater,SLOT(check()));
        QTimer::singleShot(2*60*60*1000,&updater,SLOT(check()));
    }

    QString stylePath = NcSettings::getStylePath();
    if(!stylePath.isEmpty())
    {
        QFile file(stylePath);
        if( file.open(QFile::ReadOnly) ){

            QString content = file.readAll();
            qApp->setStyleSheet(content);
        }
    }else{
        qApp->setStyleSheet("");
    }

    updateWindow();
}

void NiceCopier::updateWindow()
{
    int items = ui->tasklist->count();
    if(items>0){
        show();
        qDebug()<<"on top: "<<((this->windowFlags()&Qt::WindowStaysOnTopHint)==Qt::WindowStaysOnTopHint);
        activateWindow();
    }
    this->setUpdatesEnabled(false);
    setMaximumSize(ui->tasklist->minimumSize());
    setMinimumSize(ui->tasklist->minimumSize());
    adjustSize();

    if(items == 0)
        hide();
    else
    {
        QPoint activePos = QCursor::pos();
        int activeDesktop = QApplication::desktop()->screenNumber(activePos);
        QDesktopWidget *monitor = QApplication::desktop();
        QRect s = monitor->availableGeometry(activeDesktop);
        QPoint pos;
        switch(NcSettings::getValue<int>(NcSettings::POSITION))
        {
            case(NcSettings::TOP_RIGHT):
            {
                 pos = s.topRight();
                 pos.setX( pos.x() - frameSize().width());
            }
             break;
             case(NcSettings::TOP_LEFT):
             {
                  pos = s.topLeft();
             }
             break;
             case(NcSettings::BOTTOM_RIGHT):
             {
                  pos = s.bottomRight() - QPoint(frameSize().width(),frameSize().height());
             }
             break;
             case(NcSettings::BOTTOM_LEFT):
             {
                  pos = s.bottomLeft();
                  pos.setY( pos.y() -  this->frameSize().height());
             }
             break;
        }
        this->move(pos);
        QString title;
        title.sprintf(tr("NiceCopier: Showing %d tasks, %d running").toUtf8(),
                      NcSettings::getValue<int>(NcSettings::SHOW_TASKS),ui->tasklist->count());
        setWindowTitle(title);

    }
    this->setUpdatesEnabled(true);
}

void NiceCopier::closeEvent(QCloseEvent *evt)
{
    qDebug()<<"NiceCopier close event";
    if( evt->spontaneous() )
    {
        if(this->isHidden()){
            evt->accept();
            NcSettings::setIsExiting(true);
            QCoreApplication::exit();
            return;
        }
        evt->ignore();
        hide();
    }else
    {
        evt->accept();
        NcSettings::setIsExiting(true);
        QCoreApplication::exit();
    }
}

void NiceCopier::aboutToQuit()
{
    NcSettings::setIsExiting(true);
}

void NiceCopier::taskIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        if( this->isVisible() )
        {
            this->hide();
        }else if(taskhandler->m_count>0)
        {
            this->show();
            this->activateWindow();
        }else
            taskbar->showMessage("NiceCopier",tr("No tasks running"));
    }
}

void NiceCopier::setFirstPanel(CopyInfoPanel *panel)
{
    ui->tasklist->setFirst(panel);
}


void NiceCopier::changeEvent(QEvent *e)
{
    if( e->type()==QEvent::LanguageChange ){
        ui->retranslateUi(this);
    }
}

