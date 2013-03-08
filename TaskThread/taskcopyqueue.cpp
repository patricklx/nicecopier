

#include "taskthread.h"
#include "taskcopyqueue.h"
#include "DiskSpace.h"
#include "ReadableSize.h"
#include <QMessageBox>
#include "errormessages.h"
#include "ncsettings.h"

#include <QTextCodec>

#define BUFFER_SIZE_MAX 4*1024*1024

int queue_interval_pause=0;

TaskCopyQueue::TaskCopyQueue(TaskThread* thread)
{
    copyThreadInfo = thread;
    buffer = NULL;
    buffersize = 1024 * 1024;
    actFile = NULL;
    resume = false;
}

TaskCopyQueue::~TaskCopyQueue()
{
    if( buffer != NULL ){
        free(buffer);
        buffer = NULL;
    }
}

void TaskCopyQueue::setSpeed(int speed)
{
    queue_interval_pause = (10.0-speed)/10.0*1000;
}


void TaskCopyQueue::setFilePos(QPoint point)
{
    filePos = point;
}


File* TaskCopyQueue::getCurrentFile() const
{
    return actFile;
}


bool TaskCopyQueue::checkDiskSpace(QString fDestName)
{
    QString Destination;
    QFileInfo f(fDestName);
    Destination = f.path();
    bool ok = false;
    while(1)
    {
        double disksize=0;
        if( getFreeTotalSpace(Destination,NULL,&disksize) )
        {
            double stillNeeded = copyThreadInfo->getTotalSize()
                    -copyThreadInfo->getFileSizeCopied()
                    -copyThreadInfo->getCopyDirectory()->getReplaceSize();
            if( disksize < stillNeeded )
            {
                QString msg = tr("Error:\n not enough size in destination:\"")+Destination
                        +tr("\"\n You need at least: ") + Util::toReadableSize(stillNeeded);
                int ans;
                copyThreadInfo->sendMessage(msg,&ans,QMessageBox::Retry|QMessageBox::Cancel);
                if(ans == QMessageBox::Retry )
                {
                    ok = true;
                    continue;
                }
                else
                {
                    copyThreadInfo->exit();
                    return false;
                }
            }else
                return ok;
        }else
            return false;
    }
}


bool TaskCopyQueue::resumeQueueToFile(QString last_file,Folder *folder)
{
    if(folder == NULL){
        folder = copyThreadInfo->getCopyDirectory();
    }

    folder->setCopyStarted();

    foreach(File *file,folder->getFileList())
    {
        if( file->getCopyHandle() != File::COPY_IGNORE)
            file->setDestinationName(folder->getDestPath() + file->getDestinationName());

        if( file->getSourceName() == last_file.toUtf8() )
        {
            file->setFirst();
            return true;
        }


        if(!file->wasCopied())
            return true;

        file->setSkip(true);
    }


    foreach(Folder *subfolder,folder->getSubFolderList())
    {
        if( !resumeQueueToFile(last_file,subfolder) ){
            return false;
        }
    }
    resume = true;
    return true;
}


void TaskCopyQueue::runCheckSumTest(Folder* folder)
{
    if( folder == NULL )
        folder = copyThreadInfo->getCopyDirectory();

    if( copyThreadInfo->testDestroy() ){
        return;
    }

    foreach(File *file,folder->getFileList())
    {
        if( copyThreadInfo->testDestroy() )
            return;

        if( file->wasCopied() ){
            bool ok = file->verifyChecksum();
            if( !ok ){
                failed_copys.append(file);
            }
        }
    }

    foreach(Folder *subfolder,folder->getSubFolderList())
    {
        if( copyThreadInfo->testDestroy() )
            return;

        runCheckSumTest(subfolder);

    }
}

void TaskCopyQueue::recursiveCopy(Folder *folder)
{
    //test if the task should stop
    if(copyThreadInfo->testDestroy()){
        return;
    }

    //get the folder from the queue
    if(folder->getCopyHandle()==Folder::COPY_IGNORE )
    {
        return;
    }

    folder->setCopyStarted();

    QDir dir(folder->getDestPath());
    while(true)
    {
        if(copyThreadInfo->testDestroy())
            return;

        if( !dir.exists() )
        {
            bool ok = dir.mkdir(folder->getDestPath());
            if(!ok)
            {
                int answer;
                copyThreadInfo->sendMessage(tr("Failed to create directory:\n")+folder->getDestPath()+tr("\n Retry?"),
                                            &answer,
                                            QMessageBox::Cancel|QMessageBox::Yes
                                            );
                if(answer==QMessageBox::Cancel)
                    copyThreadInfo->exit();

                continue;
            }


            qDebug()<<"TaskCopyQueue: attributes from: " << folder->getSourcePath();
            DWORD attr = GetFileAttributes(folder->getSourcePath().toStdWString().c_str());
            QString binary; binary.setNum(attr,2);
            qDebug()<<"TaskCopyQueue: folder attributes"<<binary;
            if( attr != INVALID_FILE_ATTRIBUTES){
                SetFileAttributes(folder->getDestPath().toStdWString().c_str(),attr);
            }else
                qWarning()<<"TaskCopyQueue: INVALID_FILE_ATTRIBUTES-> last error:"<<GetLastError();
            break;
        }
        break;
    }


    //loop through all files in the folder
    QList<File*> fileList = folder->getFileList();
    File *file;
    foreach(file,fileList)
    {
        if(copyThreadInfo->testDestroy())
            break;

        if( folder->getCopyHandle() == Folder::COPY_IGNORE_EXISTING &&
                file->exists() && file->getCopyHandle() == File::COPY_NOT_SET  )
        {
            file->setCopyHandle(File::COPY_IGNORE);
        }

        if(file->getCopyHandle() == File::COPY_IGNORE
                || file->shouldSkip()
                || file->wasCopied())
        {
            copyThreadInfo->setFilesIgnored();
            continue;
        }

        if( folder->getCopyHandle() == Folder::COPY_REPLACE && file->getCopyHandle() == File::COPY_NOT_SET)
            file->setCopyHandle(File::COPY_REPLACE);

        copyThreadInfo->aquireMutex();
        actFile = file;
        copyThreadInfo->releaseMutex();

        copyThreadInfo->setCurrentFile( file->getDestinationName().afterLast("/") );
        if( file->getDestinationName().indexOf("/") == -1 )
            file->setDestinationName(folder->getDestPath() + file->getDestinationName());

        copyThreadInfo->setCurrentDirectory(folder->getSourcePath(),folder->getDestPath());

        if(QFile::exists(file->getDestinationName()) &&
                (file->getCopyHandle() != File::COPY_REPLACE) && !file->isFirst())
        {
            file->setDestinationName(file->getDestinationName().afterLast("/"));
            file->ignore(true);
            copyThreadInfo->setFilesIgnored();
            continue;
        }

        file->setBuffer(buffer,buffersize);


        if( !file->startCopyFile() )
        {
            //copy failed
            if( checkDiskSpace( file->getDestinationName() ) )
            {
                //copy again if it was caused by insuficient disk space, and if it's resolved
                buffersize = file->getBufferSize();
                continue;
            }

            //set default to retry
            file->setRetry(true);
            QThread::msleep(500);
            //else append it to failed_copys queue
            failed_copys.append(file);
        }else
        {
            //Copy success
            buffersize = file->getBufferSize();
            if(!copyThreadInfo->testDestroy())
            {
                copyThreadInfo->updateFileSizeSave();
                copyThreadInfo->addFileCount(-1);
            }
        }
        if(queue_interval_pause>0)
        {
            QThread::msleep(queue_interval_pause);
        }
        folder->setFilesCopied();
    }

    //put subfolders in queue

    foreach(Folder *subfolder,folder->getSubFolderList())
    {
        if( subfolder->getCopyHandle() == Folder::COPY_NOT_SET )
            subfolder->setCopyHandle( folder->getCopyHandle() );

        if(copyThreadInfo->testDestroy())
            break;
        this->recursiveCopy(subfolder);
    }
}

void TaskCopyQueue::startCopyQueue()
{
    //Initialize copy queue

    this->buffer = (char*)malloc(BUFFER_SIZE_MAX*sizeof(char));
    if(this->buffer==NULL)
    {
        qCritical("failed to create buffer");
        return;
    }

    this->recursiveCopy(copyThreadInfo->getCopyDirectory());

    qDebug("TaskCopyQueue: recursive copy finished");

    if( this->copyThreadInfo->testDestroy() ){
        return;
    }

    if( copyThreadInfo->shouldVerifyChecksum() ){

        copyThreadInfo->state = TaskThread::VerifyChecksum;
        copyThreadInfo->totalFiles = copyThreadInfo->getCopyDirectory()->getCopiedFilesCount();
        copyThreadInfo->totalSizeDone = 0;
        this->runCheckSumTest(copyThreadInfo->getCopyDirectory());
    }

    //process failed_copys queue
    if(!this->failed_copys.isEmpty() && !copyThreadInfo->testDestroy())
    {
        this->failedCopys_Retry();
    }
}


void TaskCopyQueue::failedCopys_Retry()
{
    copyThreadInfo->sendCopyErrorMessage(failed_copys,DlgErrorMessages::ER_COPY);

    int i = 0;

    while( failed_copys.count() > i)
    {
        File *file = (File*) failed_copys[i];
        if(file->getCopyHandle() == File::COPY_IGNORE)
        {
            copyThreadInfo->addSizeDone(-file->getCopiedSize());

            failed_copys.removeAt(i);
            copyThreadInfo->setFilesIgnored();
            continue;
        }


        actFile = file;
        QFileInfo fname(QString(file->getDestinationName()));
        copyThreadInfo->setCurrentFile( fname.fileName() );
        file->setBuffer(buffer,buffersize);

        if( !file->startCopyFile() )
        {
            QThread::msleep(500);
            file->setRetry(true);
            if( checkDiskSpace(QString(file->getDestinationName())) )
            {
                buffersize = file->getBufferSize();
                continue;
            }
            i++;
        }else
        {
            failed_copys.removeAt(i);
            copyThreadInfo->addFileCount(-1);
            buffersize = file->getBufferSize();
            if(!copyThreadInfo->testDestroy())
                copyThreadInfo->updateFileSizeSave();
        }

        if(copyThreadInfo->testDestroy())
            return;
    }

    if(failed_copys.count() > 0 && !copyThreadInfo->testDestroy()){
        failedCopys_Retry();
    }
}


