#include "copyinfopanel.h"
#include "ui_copyinfopanel.h"
#include "TaskThread/ReadableSize.h"
#include "ncsettings.h"
#include "errormessages.h"

#include <QTime>
#include <QPushButton>
#include <QMetaType>
#include <QMessageBox>
#include <QFileDialog>
#include "filelistwidget.h"
#include <QInputDialog>
#include <QFileIconProvider>
#include <QDesktopWidget>
#include <QModelIndex>
#include <QModelIndexList>
#include <QThreadStorage>

class myQMessageBox : public QMessageBox
{
    public:

        myQMessageBox(QWidget *parent = 0):QMessageBox(parent)
        {
            setParent(NULL);
            Qt::WindowFlags flag = 0;
            if(NcSettings::getValue<bool>(NcSettings::ALWAYS_ON_TOP))
                flag = Qt::WindowStaysOnTopHint;
            this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowMinMaxButtonsHint|flag);
        }
        void closeEvent(QCloseEvent *event)
        {
            if(event->spontaneous())
            {
                event->setAccepted(false);
            }else
                event->accept();
        }
};







CopyInfoPanel::CopyInfoPanel(QWidget *parent, TaskListHandler *listHandler) :
    QFrame(parent),
    ui(new Ui::copyInfoPanel)
{
    this->hide();
    QString styleTmp = qApp->styleSheet();
    qApp->setStyleSheet("");
    ui->setupUi(this);

    copyTasksHandler = listHandler;

    if(NcSettings::getValue<bool>(NcSettings::USE_PLASTIQUE_STYLE))
    {
        QPalette palette = ui->progressBar->palette();
        palette.setColor(QPalette::Highlight,NcSettings::getValue<QColor>(NcSettings::PB_COLOR));
        ui->progressBar->setPalette(palette);
    }

    button = new QPushButton(ui->groupBox);
    ui->infoWidget->hide();
    ui->editGroupBox->setChecked(false);
    ui->editGroupBox->setToolTip("You must wait until preparation is done");
    adjustSize();
    connect(&updateTimer,SIGNAL(timeout()),SLOT(timerUpdateGui()));


    ui->progressBar->setMinimumSize(ui->progressBar->size());
    this->setMinimumSize(size());

    ui->fileSizeLabel->clear();
    ui->fileSizeLabel->hide();
    ui->currentFileLabel->clear();
    ui->fileCountLabel->clear();
    ui->fromPathLabel->clear();
    ui->targetPathLabel->clear();
    ui->sizeLabel->clear();
    ui->speedLabel->clear();
    ui->totalSizeLabel->clear();
    ui->ofLabel->clear();


    button->resize(25,25);
    button->setMinimumSize(25,25);
    button->setMaximumSize(25,25);
    QIcon icon1(":/icons/expand.ico");
    button->setIcon(icon1);
    QPoint p = rect().topRight();
    p.setY(0);
    p.setX(p.x());
    button->move( p );
    button->show();

    connect(button,SIGNAL(clicked()),SLOT(expand()));

    qRegisterMetaType<FileList>("FileList");

    task = NULL;
    startedByUser = false;
    stoppedByUser = false;
    error_message = false;
    average_speed = 0;
    intermed_size = 0;
    intermed_speed = 0;
    update_filedone = 0;

    qApp->setStyleSheet(styleTmp);
}

void CopyInfoPanel::closeEvent(QCloseEvent *evt)
{
    if(evt->spontaneous())
    {
        evt->setAccepted(false);
        contract();
    }
}

void CopyInfoPanel::expand()
{
    button->setIcon(QIcon(":/icons/contract.ico"));
    disconnect(button,SIGNAL(clicked()),this,SLOT(expand()));
    connect(button,SIGNAL(clicked()),SLOT(contract()));

    this->setMaximumSize(16777215,16777215);
    ui->infoWidget->show();
    layout();
    adjustSize();
    this->setMinimumSize(this->minimumSize().width(),size().height());
    expand(this);
}

void CopyInfoPanel::contract()
{
    button->setIcon(QIcon(":/icons/expand.ico"));
    disconnect(button,SIGNAL(clicked()),this,SLOT(contract()));
    connect(button,SIGNAL(clicked()),SLOT(expand()));

    ui->infoWidget->hide();
    this->setMinimumSize(this->minimumWidth(),0);
    adjustSize();
    this->setMaximumSize(this->minimumWidth(),size().height());
    this->setMinimumSize(this->minimumWidth(),size().height());
    contract(this);
}

void CopyInfoPanel::resizeEvent(QResizeEvent *q)
{
    Q_UNUSED(q);
    QPoint p = rect().topRight();
    p.setY(p.y()+6);
    p.setX(p.x()-24);
    button->move( p );
    button->show();
}


CopyInfoPanel::~CopyInfoPanel()
{
    if( task != NULL )
    {
        task->disconnect();
        task->exit(true);
        if(task->isRunning())
        {
            if( task->wait(3000) )
                delete task;
        }else
            delete task;
        task = NULL;
    }
    delete button;
    delete ui;
}


void CopyInfoPanel::copyError(TaskThread *thread,FileList list,int type)
{
    DlgErrorMessages *errordlg = new DlgErrorMessages(this);
    errordlg->setErrors(thread,list,type);
    errordlg->show();
    errordlg->exec();

    if(NcSettings::isExiting())
    {
        task->exit(true);
        task->disconnect();
    }

    if(!NcSettings::isExiting())
    {
        if( task->isExiting() )
        {
            stoppedByUser = true;
        }
    }

    delete errordlg;
    ui->copyFileTree->showAll();
}

void CopyInfoPanel::threadMessage(QString msg,int *answer,int buttons)
{
    myQMessageBox message(this);
    message.setWindowTitle("NiceCopier message");
    message.setStandardButtons((QMessageBox::StandardButton)buttons);
    message.setText("Source: " + task->getSourcePath()+
                    "\nDestination: " + task->getDestinationPath());

    message.setInformativeText(msg);
    message.move(NcSettings::screenCenter()-message.rect().bottomRight()/2);
    message.show();

    *answer = message.exec();


    if(NcSettings::isExiting())
    {
        task->exit(true);
        task->disconnect();
    }
}

void CopyInfoPanel::errorMessage(TaskThread *thread, int error)
{
    QList<TreeItem*> items;
    if( error == DlgErrorMessages::ER_FILE_DEST_EXIST)
    {
        foreach(Folder *f, task->getCopyDirectory()->getSubFolderList())
        {
            setDefaultHandle(f->getTreeItem());
            items.append(f->getTreeItem());
        }

        foreach(File *f, task->getCopyDirectory()->getFileList())
        {
            setDefaultHandle(f->getTreeItem());
            items.append(f->getTreeItem());
        }
    }

    ui->copyFileTree->setTopList(items,task);

    DlgErrorMessages *errordlg = new DlgErrorMessages(this);
    int errors = errordlg->setErrors(thread,error);
    if(errors)
    {
        error_message = true;
        errordlg->show();
        errordlg->exec();
        error_message = false;
    }


    if(NcSettings::isExiting())
    {
        task->exit(true);
        task->disconnect();
    }
    delete errordlg;

    ui->copyFileTree->showAll();
}

void CopyInfoPanel::taskStarting()
{
    updateTimer.start(100);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->reset();
    ui->editGroupBox->setChecked(false);

    ui->ofLabel->setText("Size: ");
    ui->speedLabel->setText("Files: ");
    ui->startButton->setIcon(QIcon(":/icons/pause_copy.ico"));
    ui->groupBox->setTitle("preparing...");

    if(!startedByUser)
        startEvent(this);
}


void CopyInfoPanel::setDefaultHandle(TreeItem *treeitem)
{
    if( treeitem->data(COL_EXISTS).isValid() )
    {
        switch(NcSettings::getValue<int>(NcSettings::DEFAULT_EXIST_HANDLE))
        {
            case NcSettings::HANDLE_REPLACE:
                treeitem->replace(true);
            break;
            case NcSettings::HANDLE_RENAME:
                treeitem->rename(true);
            break;
            case NcSettings::HANDLE_IGNORE:
                treeitem->ignoreExisting(true);
            break;
        }
    }
}

void CopyInfoPanel::taskPrepared()
{
    ui->progressBar->setMaximum(1000);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->update();
    ui->totalSizeLabel->setText( Util::toReadableSize( task->getTotalSize()) );
    ui->ofLabel->setText("of");
    ui->sizeLabel->setText("0B");
    task->updateTotalFiles();
    ui->fileCountLabel->setNum(task->getTotalFiles());
    layout();
    ui->editGroupBox->setToolTip("");

    QList<TreeItem*> items;

    foreach(Folder *f, task->getCopyDirectory()->getSubFolderList())
    {
        items.append(f->getTreeItem());
    }

    foreach(File *f, task->getCopyDirectory()->getFileList())
    {
        items.append(f->getTreeItem());
    }

    ui->copyFileTree->setTopList(items,task);
    ui->copyFileTree->showAll();

    if(ui->editCheckBox->isChecked())
    {
        if(!task->isPaused())
            on_startButton_clicked();//pause
        if(!ui->infoWidget->isVisible())
            expand();
    }
}

void CopyInfoPanel::taskCopyDone()
{
    if(task->isDeleteDestinationSet())
        return;

    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->sizeLabel->setText(Util::toReadableSize(task->getTotalSize()));
    ui->fileCountLabel->clear();

    ui->fileSizeLabel->clear();
    ui->fileSizeLabel->hide();


    if(task->getCopyOperation() == TaskThread::OP_MOVE)
    {
        updateTimer.stop();
        int time = NcSettings::getValue<int>(NcSettings::DELETE_TIME)*1000;
        QString str_time;
        str_time.setNum(time);
        ui->groupBox->setTitle("Deleting Source in " + str_time + " sec");
        while(time > 0 && !task->isExiting())
        {
            time -= 50;
            QThread::msleep(50);
            QApplication::processEvents();
            if(NcSettings::isExiting())
            {
                task->exit(true);
                task->disconnect();
                break;
            }
            str_time.setNum(time/1000+1);
            ui->groupBox->setTitle("Deleting Source in " + str_time + " sec");
        }
        task->updateTotalFiles();
        ui->sizeLabel->clear();
        ui->speedLabel->clear();
        ui->totalSizeLabel->clear();
        ui->ofLabel->setText("Deleting: ");
        ui->progressBar->setMaximum(task->getTotalFiles());
        ui->progressBar->setValue(0);
        ui->groupBox->setTitle("Deleting...");
        layout();

        updateTimer.start();
        return;
    }

    ui->groupBox->setTitle("Copy Done");
}

void CopyInfoPanel::taskfinished()
{
    updateTimer.stop();
    ui->progressBar->setValue(ui->progressBar->maximum());

    ui->groupBox->setTitle("Copy Done");
    ui->startButton->setDisabled(true);
    ui->stopButton->setDisabled(true);
    if(task)
    {
        if(!NcSettings::isExiting())
        {
            if( task->isExiting() )
            {
                stoppedByUser = true;
            }
        }

        if(!stoppedByUser)
            copyTasksHandler->threadTerminated(task);
        else
            copyTasksHandler->remove(task);

        sourceList = task->getSourceList();
        source = task->getSourcePath();
        destination = task->getDestinationPath();
    }

    delete task;
    task = NULL;

    if(!this->isVisible())
        finishedEvent(this);
    else
        QTimer::singleShot(500,this,SLOT(sendDelayedFinished()));

}

void CopyInfoPanel::on_startButton_clicked()
{
    if(task == NULL)
        return;

    //Update total size after editing
    if( !task->isPreparing()
        && !task->isDeletingDestination()
        && !task->isDeletingSource())
    {
        ui->totalSizeLabel->setText( Util::toReadableSize( task->getTotalSize() ) );
        //get to be copied files count
        task->updateTotalFiles();
        ui->fileCountLabel->setNum(task->getTotalFiles());
    }

    if(!task->isRunning())
    {
        task->resume();
        task->start();
        startedByUser = true;
        ui->startButton->setIcon(QIcon(":/icons/pause_copy.ico"));
        return;
    }

    if(!task->isPaused())
    {
        task->pause();
        updateTimer.start(1000);
        ui->startButton->setIcon(QIcon(":/icons/start_copy.ico"));
        if(!task->isPreparing()
           && !task->isDeletingDestination()
           && !task->isDeletingSource()
           && task->getCurrentState()!=TaskThread::VERIFY_CHECKSUM){

            ui->editGroupBox->setChecked(true);
        }

        File *file = task->getCurrentFile();
        if( file != NULL )
        {
            ui->copyFileTree->scrollTo(file->getTreeItem());
        }
        return;
    }else
    {
        task->resume();
        updateTimer.start(100);
        ui->startButton->setIcon(QIcon(":/icons/pause_copy.ico"));
        ui->editGroupBox->setChecked(false);

        if(!task->isPreparing())
        {
            ui->editCheckBox->setCheckable(false);
            task->disableEdit();
        }
        return;
    }
}

void CopyInfoPanel::on_stopButton_clicked()
{
    if(!task)
    {
        return;
    }

    bool paused = task->isPaused();

    task->pause();
    updateTimer.stop();

    if(NcSettings::getValue<bool>(NcSettings::CONFIRM_CANCEL))
    {
        int ans = QMessageBox::question(this,"Confirm Cancel","Do you really want to cancel this task?",QMessageBox::Yes|QMessageBox::No);
        if(ans == QMessageBox::No)
        {
            if(task && !paused)//be sure that the task still exists
            {
                task->resume();
                updateTimer.start();
            }
            return;
        }
    }

    //remove items to prevent a crash
    ui->copyFileTree->clear();

    if(NcSettings::getValue<bool>(NcSettings::DELETE_DEST_CANCEL)
       && !task->isPreparing()
       && task->getCopyDirectory()->hasCopyStarted())
    {
        bool confirmed = false;
        if(NcSettings::getValue<bool>(NcSettings::CONFIRM_DELETE))
        {
            int ans = QMessageBox::question(this,"Confirm Delete","Delete all copied files?",QMessageBox::Yes|QMessageBox::No);
            if(ans == QMessageBox::Yes)
                confirmed = true;
        }else
            confirmed = true;

        if(confirmed)
        {
            if(task)//be sure that the task still exists
            {
                task->enableDeleteDestination();
                ui->sizeLabel->clear();
                ui->speedLabel->clear();
                ui->totalSizeLabel->clear();
                ui->ofLabel->setText("Deleting: ");
                ui->groupBox->setTitle("Deleting...");
                task->updateTotalFiles();
                ui->progressBar->setMaximum(task->getTotalFiles());
                ui->progressBar->setValue(0);
            }
        }
    }
    stoppedByUser = true;
    if(task)
    {
        if(!task->isRunning())
        {
            taskfinished();
            return;
        }
        task->exit();
    }
    updateTimer.start(50);
}


bool CopyInfoPanel::createTask( QIODevice &info )
{

    if(NcSettings::getValue<bool>(NcSettings::USE_PLASTIQUE_STYLE))
    {
        QPalette palette = ui->progressBar->palette();
        palette.setColor(QPalette::Highlight,NcSettings::getValue<QColor>(NcSettings::PB_COLOR));
        ui->progressBar->setPalette(palette);
    }

    task = new TaskThread( info );
    if(task->getCopyOperation()==TaskThread::OP_MOVE)
    {
        ui->copyRadioButton->setChecked(false);
        ui->cutRadioButton->setChecked(true);
    }
    if( NcSettings::getValue<bool>(NcSettings::TEST_CHECKSUM) ){

        ui->checkSumCheckBox->setChecked(true);
        on_checkSumCheckBox_toggled(true);
    }
    connect(task,SIGNAL(starting()),SLOT(taskStarting()));
    connect(task,SIGNAL(copyDone()),SLOT(taskCopyDone()),Qt::BlockingQueuedConnection);
    connect(task,SIGNAL(message(QString,int*,int)),SLOT(threadMessage(QString,int*,int)),Qt::BlockingQueuedConnection);
    connect(task,SIGNAL(finished()),SLOT(taskfinished()));
    connect(task,SIGNAL(prepared()),SLOT(taskPrepared()),Qt::BlockingQueuedConnection);
    connect(task,SIGNAL(prepareErrorMessage(TaskThread*,int)),SLOT(errorMessage(TaskThread*,int)),Qt::BlockingQueuedConnection);
    connect(task,SIGNAL(copyErrorMessage(TaskThread*,FileList,int)),SLOT(copyError(TaskThread*,FileList,int)),Qt::BlockingQueuedConnection);

    int id = task->isFileLoaded();
    if( id != -1 )
    {
        this->show();
        if(task->isEditEnabled())
            ui->editCheckBox->setChecked(Qt::Checked);

        if( task->getFileSizeCopied() != 0 && task->getTotalSize() != 0)
        {
            ui->ofLabel->setText("of");
            ui->sizeLabel->setText( Util::toReadableSize( task->getTotalSize() ) + " " );
            ui->fileCountLabel->setNum( task->getTotalFiles() );
            updateSize();
            layout();
        }
        ui->fromPathLabel->setText(task->getSourcePath());
        ui->fromPathLabel->setToolTip(task->getSourcePath());

        ui->targetPathLabel->setText(task->getDestinationPath());
        ui->targetPathLabel->setToolTip(task->getDestinationPath());

        ui->currentFileLabel->setText( task->getCurrentFileName() );
        ui->currentFileLabel->setToolTip( task->getCurrentFileName() );


        if( copyTasksHandler->add( task ) ) //returns true if task is started immediatily
        {
            layout();
            return true;
        }

        //was started and not been paused when NiceCopier closed
        if(task->hasStarted() && !task->isPaused())
        {
            task->start();
            return true;
        }

        if(!task->isPreparing())
        {
            //Don't expand on creating, set edit to false and after calling
            //TaskPrepared() reset the value
            bool checked = ui->editCheckBox->isChecked();
            ui->editCheckBox->setChecked(false);

            taskPrepared();

            updateTimer.start(1000);

            ui->editCheckBox->setChecked(checked);
            ui->editGroupBox->setChecked(true);
        }

        QGroupBox *box = ui->groupBox;
        int type =  task->getCopyType();
        switch( type )
        {
            case(TaskListHandler::DISK_TO_DISK):
            {
                box->setTitle("Enqueued - Disk to Disk" );
            }
            break;
            case(TaskListHandler::LOCAL_NETWORK):
            {
                box->setTitle("Enqueued - LAN" );
            }
            break;
            case(TaskListHandler::TO_INET):
            {
                box->setTitle("Enqueued - To internet location" );
            }
            break;
            case(TaskListHandler::FROM_INET):
            {
                box->setTitle("Enqueued - From internet location" );
            }
            break;
            case(TaskListHandler::FROM_CDDVD):
            {
                box->setTitle("Enqueued - From CD/DVD" );
            }
            break;
            default:
                box->setTitle("Enqueued");
        }

        if(task->getCopyOperation()==TaskThread::OP_MOVE)
            box->setTitle(box->title()+ " - MOVE" );
        else
            box->setTitle(box->title()+ " - COPY" );
        return true;
    }
    else
    {
        delete task;
        task = NULL;
        return false;
    }
    return true;
}

int CopyInfoPanel::getTaskId()
{
    if(isTaskValid())
        return task->getTaskId();
    else
        return -1;
}

bool CopyInfoPanel::isTaskValid()
{
    return task!=NULL;
}

bool CopyInfoPanel::isStoppedByUser()
{
    return stoppedByUser;
}

QString CopyInfoPanel::getSourceListMsg()
{
    QString msg;
    for(int i=0;i<5 && i<sourceList.count();i++)
    {
        QStringExt source = sourceList[i].afterLast('/');
        if(source.isEmpty())
            source = sourceList[i].beforeLast('/').afterLast('/')+QStringExt('/');
        msg = msg + "\n" + source;
    }
    if(sourceList.count()>5)
        msg += "\n...";

    msg +=  tr("\n\n") +
            "From:   " + source + "\n" +
            "To:     " + destination;
    return msg;
}


void CopyInfoPanel::timerUpdateGui()
{
    task->aquireMutex();
    if( task == NULL )
    {
        updateTimer.stop();
        task->releaseMutex();
        return;
    }

    if( task->getCurrentFileName() != fileDoneUpdate )
    {
        update_filedone = 1500;
        ui->fileSizeLabel->clear();
        ui->fileSizeLabel->hide();
        fileDoneUpdate = task->getCurrentFileName();
        watchfiledone.restart();
    }

    ui->targetPathLabel->setText(task->getCurrentDestinationFolder());
    ui->fromPathLabel->setText(task->getCurrentSourceFolder());


    if(ui->groupBox->title()!="Deleting...")
    {
        if( watchSize.elapsed() > 99 )
        {
            if( task->isPreparing() )
                updateSearchfiles();
            else
                updateSize();
            watchSize.restart();
        }

        if( !task->isPreparing() && !task->isPaused() )
        {
            if( watchSpeed.elapsed() > 3000 )
            {
                updateSpeed(watchSpeed.elapsed());
                watchSpeed.restart();
            }
            if( watchTime.elapsed() > 1000 )
            {
                updateTime();
                watchTime.restart();
            }
            if( adjustSpeed.elapsed() > 10000 )
            {
                calcMidSpeed(adjustSpeed.elapsed());
                adjustSpeed.restart();
            }

            if( watchfiledone.elapsed() > update_filedone )
            {
                updateFileDone();
                update_filedone = 199;
                watchfiledone.restart();
            }
        }
    }

    if(task->isDeletingDestination() || task->isDeletingSource())
    {
        ui->groupBox->setTitle("Deleting...");
        ui->progressBar->setValue( ui->progressBar->maximum() - task->getTotalFiles());
        ui->fileCountLabel->setNum(task->getTotalFiles());
        ui->currentFileLabel->setText(task->getCurrentFileName());
    }
    this->layout();
    task->releaseMutex();
}

void CopyInfoPanel::updateSearchfiles()
{
    ui->totalSizeLabel->setText( Util::toReadableSize( task->getTotalSize() )+" " );
    if(error_message)
    {
        //get to be copied files count
        task->updateTotalFiles();
    }
    ui->fileCountLabel->setNum( task->getTotalFiles() );
    ui->currentFileLabel->setText( task->getCurrentFileName() );
    ui->currentFileLabel->setToolTip( task->getCurrentFileName() );
}



inline QColor GetContrastingTextColor(const QColor & c)
{
    int a = qMax(c.red(),c.blue());
    a = qMax(a,c.blue());
    return (a >= 128) ? Qt::black : Qt::white;
}

void CopyInfoPanel::updateSize()
{
    int  perc = 1000.0 * task->getFileSizeCopied() / task->getTotalSize();

    if(ui->editGroupBox->isChecked() || error_message)
    {
        task->updateTotalFiles();
        ui->totalSizeLabel->setText( Util::toReadableSize( task->getTotalSize() )+" " );
    }

    ui->progressBar->setValue( perc );

    ui->sizeLabel->setText(Util::toReadableSize( task->getFileSizeCopied() ));
    QString count; count.setNum( task->getTotalFiles() );
    ui->fileCountLabel->setText(  " (" + count + ")"  );

    ui->currentFileLabel->setText( task->getCurrentFileName() );
    ui->currentFileLabel->setToolTip( task->getCurrentFileName() );
}

void CopyInfoPanel::updateTime()
{
    if( !task->getActFileError().isEmpty() )
    {
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        ui->groupBox->setPalette(palette);

        ui->groupBox->setTitle(task->getActFileError());
        ui->groupBox->setToolTip(task->getActFileError());
        return;
    }else
    {
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        ui->groupBox->setPalette(palette);
        if( average_speed != 0 )
        {
            int timeleft = 1.0*(task->getTotalSize() - task->getFileSizeCopied()) / average_speed;
            int h = timeleft/3600;
            int m = (timeleft % 3600) /60;
            int s = (timeleft % 3600) % 60;

            QString t;
            if( h != 0 )
                t.sprintf("%dh:%dm:%ds",h,m,s);
            else
                if( m != 0 )
                    t.sprintf("%dm:%ds",m,s);
                else
                    if( s != 0 )
                        t.sprintf("%ds",s);

            if( task->getCurrentState()!=TaskThread::VERIFY_CHECKSUM ){
                if(task->getCopyOperation()==TaskThread::OP_MOVE)
                    ui->groupBox->setTitle("TIME LEFT: " + t + " - MOVE");
                else
                    ui->groupBox->setTitle("TIME LEFT: " + t + " - COPY");
            }else{
                ui->groupBox->setTitle("TIME LEFT: " + t + " - Verify checksum");
            }
        }
    }
}

void CopyInfoPanel::updateFileDone()
{
    double done = task->getActFileSize()-task->getActFileCpySize() ;

    ui->fileSizeLabel->setText(Util::toReadableSize(done));
    ui->fileSizeLabel->show();
    this->layout();
    File *file = task->getCurrentFile();
    if( file != NULL )
    {
        ui->copyFileTree->updateItem(file->getTreeItem());
    }
}

void CopyInfoPanel::updateSpeed( int time )
{
    size_t intermed_speed =  1000.0*(task->getTotalCopiedSize() - intermed_size) / time;

    if( average_speed != 0 )
        average_speed = ( average_speed + intermed_speed ) / 2;
    else
        average_speed = intermed_speed;
    QString speed = Util::toReadableSize( average_speed );

    ui->speedLabel->setText(speed);
    intermed_size = task->getTotalCopiedSize();

}

void CopyInfoPanel::calcMidSpeed( int time )
{
    average_speed =  1000.0*(task->getTotalCopiedSize() - intermed_speed) / time;
    intermed_speed = task->getTotalCopiedSize();
}



void CopyInfoPanel::on_addFileButton_clicked()
{
    QModelIndexList itemlist = ui->copyFileTree->getSelected();

    if(itemlist.count() > 1)
    {
        QMessageBox::information(this,"Select one item","Select only one item or none");
        return;
    }

    QFileDialog filedialog(this);
    filedialog.exec();

    if(filedialog.result() == false)
        return;

    QStringList filelist = filedialog.selectedFiles();

    for(int i=0;i<filelist.count();i++)
    {
        QFileInfo file(filelist[i]);
        if(itemlist.count() == 0)
        {
            task->getCopyDirectory()->addFile(file);
        }else
        {
            TreeItem *parentitem = (TreeItem*)  (itemlist[0]).internalPointer();
            if( !parentitem->getFolder() )
            {
                parentitem = parentitem->parent();
            }
            if(parentitem==NULL)
            {
                task->getCopyDirectory()->addFolder(file);
            }
            else
            {
                Folder *folder = parentitem->getFolder();
                ui->copyFileTree->getModel()->setBeginInsertRows(parentitem,folder->getSubFolderList().count());
                folder->addFile(file);
                ui->copyFileTree->getModel()->setEndInsertRows();
            }
        }
    }
    updateSearchfiles();
}

void CopyInfoPanel::on_addFolderButton_clicked()
{
    QModelIndexList itemlist = ui->copyFileTree->getSelected();
    if(itemlist.count() > 1)
    {
        QMessageBox::information(this,"Select one item","Select only one item or none");
        return;
    }

    //Get the folder
    QMessageBox question(this);
    question.setWindowTitle("Add folder");
    question.setText("Do you want to create a new folder or choose an existing one?");
    question.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    question.setButtonText(QMessageBox::Yes,"New Folder");
    question.setButtonText(QMessageBox::No,"Existing folder");

    QStringList folders;
    bool existing = false;
    //Create a new folder
    int ans = question.exec();
    if(ans==QMessageBox::Yes)
    {
        QInputDialog input(this);
        input.setLabelText("Set the name of the new folder");
        if(input.exec())
        {
            folders.append(input.textValue());
        }
        else
            return;
    }else if(ans==QMessageBox::No)//Choose existing folder
    {
        QFileDialog filedialog(this);
        filedialog.setWindowTitle("choose directories");
        filedialog.setFileMode(QFileDialog::Directory);
        filedialog.setOption(QFileDialog::ShowDirsOnly,true);
        if( filedialog.exec() )
        {
            folders = filedialog.selectedFiles();
        }else
            return;

        existing = true;
    }else
        return;

    for(int i=0;i < folders.count();i++)
    {

        Folder *subfolder = NULL;
        if(itemlist.count() == 0)
        {
            //Add to top level
            subfolder = task->getCopyDirectory()->addFolder(QFileInfo(folders[i]));
            ui->copyFileTree->addTopLevelItem(subfolder->getTreeItem());
        }else
        {
            //Get parent
            TreeItem* parentitem = (TreeItem*) itemlist[0].internalPointer();

            if( !parentitem->getFolder() )
            {
                parentitem = parentitem->parent();
            }

            if(parentitem==NULL)
            {
                //Add to top level
                subfolder = task->getCopyDirectory()->addFolder(QFileInfo(folders[i]));
                ui->copyFileTree->addTopLevelItem(subfolder->getTreeItem());
            }
            else
            {
                //Add to existing folder in the tree

                ui->copyFileTree->getModel()->setBeginInsertRows(parentitem,parentitem->childCount());

                parentitem->getFolder()->addFolder(QFileInfo(folders[i]));

                ui->copyFileTree->getModel()->setEndInsertRows();
            }
        }
        //If the folder exists in the  filesystem traverse files and subfolders
        if(existing)
        {
            subfolder->traverse(folders[i]+"/");
        }
    }
    updateSearchfiles();
}


void CopyInfoPanel::on_editGroupBox_clicked(bool checked)
{
    if(checked && !task->isPreparing() && !task->isDeletingDestination() && !task->isDeletingSource() )
    {
        if(!task->isPaused() && task->hasStarted())
            on_startButton_clicked();
    }else
        ui->editGroupBox->setChecked(false);
}

void CopyInfoPanel::on_copyRadioButton_clicked()
{
    task->setCopyOperationType(TaskThread::OP_COPY);
}

void CopyInfoPanel::on_cutRadioButton_clicked()
{
    task->setCopyOperationType(TaskThread::OP_MOVE);
}

void CopyInfoPanel::on_speedSlider_inversed_sliderMoved(int position)
{
    QString percentage=QString("%1%").arg((1.0*position)/10.0*100);
    ui->speedLevelLabel->setText(percentage);
    task->setSpeed(position);
}

void CopyInfoPanel::on_speedSlider_inversed_valueChanged(int position)
{
    QString percentage=QString("%1%").arg((1.0*position)/10.0*100);
    ui->speedLevelLabel->setText(percentage);
    task->setSpeed(position);
}

void CopyInfoPanel::on_checkSumCheckBox_toggled(bool checked)
{
    task->setCheckMd5(checked);
}
