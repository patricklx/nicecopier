
#include "taskpreparer.h"
#include "taskthread.h"
#include "errormessages.h"
#include "DiskSpace.h"
#include "ReadableSize.h"
#include <QMessageBox>
#include <QDebug>

void TaskPreparer::create(TaskThread* thread, QString source, QString dest)
{
    sourceDir = source;
    destination = dest;
    copyThreadInfo = thread;
    directory = thread->getCopyDirectory();
    sourceList = thread->getSourceList();
}

TaskPreparer::TaskPreparer(  )
{

}


void TaskPreparer::checkDestFreeSize()
{
    while(1)
    {
        if(copyThreadInfo->testDestroy())
            return;
        double disksize=0;
        if( getFreeTotalSpace(destination,NULL,&disksize) )
        {
            double replacesize = 0;
            if( disksize < copyThreadInfo->getTotalSize() )
            {
                replacesize = copyThreadInfo->getCopyDirectory()->getUsedDiskSpace();
            }

            if( disksize < copyThreadInfo->getTotalSize() - replacesize)
            {
                emit copyThreadInfo->sendPrepareErrorMessage(DlgErrorMessages::FREE_SPACE_MODE);
                continue;//check again
            }
            return;
        }
        return;
    }
}

bool TaskPreparer::prepareCopy()
{
    int count = sourceList.count();
    int i  = 0;

    //Get All files from source path
    while ( i < count )
    {
        QStringExt path = sourceList.at(i);
        QFileInfo file( path );


        if(!file.exists())
        {
            path = path.beforeLast("/");
            file.setFile(path);
        }

        qDebug()<<"add: "<<path;

        if ( file.exists() )
        {
            if ( file.isFile() || file.isSymLink() )
            {
                directory->addFile(file);
            }
            else
            {
                file = QFileInfo( path.beforeLast('/') );
                directory->addFolder(file);
            }
        }
        i++;
        if ( copyThreadInfo->testDestroy() )
            return false;
    }

    if ( !copyThreadInfo->testDestroy() )
    {
        bool found = false;

        foreach(File* file,directory->getFileList())
        {
            if(file->exists())
            {
                emit copyThreadInfo->sendPrepareErrorMessage(DlgErrorMessages::ER_FILE_DEST_EXIST);
                found = true;
                break;
            }
        }


        /*don't check for exiting folders if we already found an existing file*/
        if( !found )
        {
            foreach(Folder *folder,directory->getSubFolderList())
            {
                if(folder->exists())
                {
                    emit copyThreadInfo->sendPrepareErrorMessage(DlgErrorMessages::ER_FILE_DEST_EXIST);
                    break;
                }
            }
        }

        if(copyThreadInfo->testDestroy())
            return false;

        checkDestFreeSize();
        return true;
    }else
        return false;
}


void TaskPreparer::deleteDest(Folder *folder)
{

    foreach(Folder* subfolder, folder->getSubFolderList())
    {
        if(!subfolder->getDestPath().length() == 0)
        {
            QString fname = subfolder->getDestPath();
            QDir dir(fname);
            if( dir.exists() )
                deleteDest(subfolder);
        }
    }

    foreach(File *f,folder->getFileList())
    {
        f->deleteDest();
    }

    while(1)
    {
        QString fname = folder->getDestPath();
        QDir dir(fname);
        if (!dir.rmdir( fname))
        {

            int res;
            copyThreadInfo->sendMessage("Can't remove destination: " + fname + "\n retry?",&res,QMessageBox::Yes|
                                                                                                    QMessageBox::No|
                                                                                                    QMessageBox::Cancel);
            if ( res == QMessageBox::Ok )
            {
                continue;
            }
            if( res == QMessageBox::No )
                break;

            if ( res == QMessageBox::Cancel )
                return;
        }else
            break;

        if ( copyThreadInfo->testDestroy() )
                return;
    }
}





