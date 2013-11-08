

#include "taskthread.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QFileInfo>
#include <QString>
#include <QObject>
#include <QDir>
#include <QTime>
#include <QFileInfo>

#include <QTreeWidgetItem>
#include <QFileIconProvider>
#include "ReadableSize.h"
#include "ncsettings.h"

#include "mFile.h"
#include <windows.h>
#include <io.h>
#define MAX_SIZE  4 * 1024 * 1024
#define MIN_SIZE  64*1024

//Global instance of QFileIconProvider to make search faster
//In debug mode this will cause a crash -.-


class QTimeWatch : public QTime
{
    int time_elapsed;
public:
    QTimeWatch()
        :QTime()
    {

    }
    void pause()
    {
        time_elapsed = this->elapsed();
    }

    int time()
    {
       return time_elapsed;
    }

};

mFile::mFile(TaskThread *thread,mFolder *folder,QFileInfo fileinfo,bool check_exists)
{
    buffer = NULL;
    fDest = NULL;
    fSource = NULL;
    IsFirstInQueue = false;
    buffersize = 0;
    mCopiedSize = 0;
    average_speed = 64*1024;
    retry = false;
    copyhandle = COPY_NOT_SET;
    skip = false;
    copy_started = false;
    CopyThreadInfo = thread;
    parent_folder = folder;
    dest_exist = false;
    copied = false;

    fDestName = fileinfo.fileName().toStdWString();
    fSourceName = fileinfo.filePath().toStdWString();

    treeitem = new TreeItem(*this,this->parent_folder->treeitem);

    if(fDestName.length() == 0)
        return;

    fsize = fileinfo.size();
    thread->addFileCount(1);
    thread->addTotalSize(fsize);

    QString name = parent_folder->getDestPath() + QString::fromStdWString( fDestName );
    QFileInfo destfileinfo(name);

    if(check_exists && exists())
    {
        dest_exist = true;
        QDateTime sourceModified = fileinfo.lastModified();
        QDateTime destModified = destfileinfo.lastModified();

        if(destModified<sourceModified)
            destAge = OLDER;
        if(destModified>sourceModified)
            destAge = NEWER;
        if(destModified==sourceModified)
            destAge = EQUAL;
    }
}

mFile::~mFile()
{
    if( fDest != NULL)
        delete fDest;
    if( fSource != NULL)
        delete fSource;
    delete treeitem;
}

bool mFile::exists()
{
    QString path = parent_folder->getDestPath();
    QString name = path + QString::fromStdWString(fDestName);
    return QFile::exists(name);
}

void mFile::replace(bool replace)
{
    if(parent_folder->isReplace() || !dest_exist)
        return;

    if(replace && copyhandle!=mFile::COPY_REPLACE)
    {
        ignore(false);
        unrename();

        parent_folder->addSizeExistingFile(-fsize);
        parent_folder->addSizeReplace(fsize);
        copyhandle = mFile::COPY_REPLACE;
    }
    else if(!replace && copyhandle==mFile::COPY_REPLACE)
    {
        parent_folder->addSizeExistingFile(+fsize);
        parent_folder->addSizeReplace(-fsize);
        copyhandle = mFile::COPY_NOT_SET;
    }
}


void mFile::unrename()
{
    if(copyhandle==COPY_RENAME)
    {
        fDestName = QString::fromStdWString(fSourceName).afterLast("/").toStdWString();
        copyhandle = COPY_NOT_SET;

        if(dest_exist)
            parent_folder->addSizeExistingFile(+fsize);
    }
}

void mFile::ignoreExisting(bool m_ignore)
{
    if(dest_exist)
        ignore(m_ignore);
}

void mFile::ignore(bool m_ignore)
{
    if(parent_folder->isIgnore() || (parent_folder->isIgnoreExisting() && dest_exist) )
        return;

    if(m_ignore && copyhandle!=COPY_IGNORE)
    {
        unrename();
        replace(false);
        copyhandle = COPY_IGNORE;
        if(dest_exist)
            parent_folder->addSizeExistingFile(-fsize);
        parent_folder->addSize(-this->fsize);
    }
    else if(!m_ignore &&  copyhandle==COPY_IGNORE)
    {
        copyhandle = COPY_NOT_SET;
        if(dest_exist)
            parent_folder->addSizeExistingFile(+fsize);
        parent_folder->addSize(fsize);
    }
}

bool mFile::rename(QString new_name)
{
    if(!new_name.isEmpty())
    {
        std::wstring temp = fDestName;
        fDestName = new_name.toStdWString();
        if(exists())
        {
            fDestName = temp;
            return false;
        }
        ignore(false);
        replace(false);
        copyhandle = COPY_RENAME;
        if(dest_exist)
            parent_folder->addSizeExistingFile(-fsize);

        return true;
    }
    if(new_name.isEmpty())
        new_name = QString::fromStdWString(fDestName);


    QString path = parent_folder->getDestPath();
    int i=1;
    while(1)
    {
        QString test;
        QString infix;
        QString suffix = new_name.afterLast('.');
        if(suffix == new_name)
            suffix.clear();
        infix.sprintf(" (%d).",i);

        test = path + new_name.beforeLast('.') + infix + suffix;

        if(!QFile::exists(test))
        {
            new_name = new_name.beforeLast('.') + infix + suffix;
            break;
        }
        i++;
    }
    fDestName = new_name.toStdWString();

    ignore(false);
    replace(false);
    copyhandle = COPY_RENAME;
    if(dest_exist)
        parent_folder->addSizeExistingFile(-fsize);
    return true;
}

size_t mFile::Calc_BfSize(int time)
{
    double p = 0;

    if ( time == 0 )
    {
        return 2*average_speed;
    }

    if ( time < 400 )
    {
        double sp = 2*average_speed;
	if( sp > MAX_SIZE )
            return 0;
        p = sp;
        average_speed = sp;
    }

    if ( time > 1500 )
    {
        double sp =  average_speed/2;
        if ( sp < MIN_SIZE  )
            return 0;
        p = sp;
        average_speed = sp;
    }
    qDebug("buffer: %d",average_speed);

    return p;
}

void mFile::SaveCopiedSize()
{
    QString taskfile;
    taskfile.sprintf( "Tasks/Task%i.TaskLastFile", TaskId );
    QFile file(taskfile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text ))
         return;

    QString content;
    content.setNum(mCopiedSize,'g',100);
    content = QString::fromStdWString(fSourceName) + QString("\n") + content;

    QTextStream stream(&file);

    stream << content;
}

double mFile::GetCopiedSize()
{
    double size;
    QString taskfile;
    taskfile.sprintf( "Tasks/Task%i.TaskLastFile", TaskId );
    QFile file(taskfile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text ))
         return 0;

    QTextStream in(&file);
    taskfile = in.readLine();
    in >> size;

    return size;
}


bool mFile::SetFileSize()
{
    HANDLE HFile = (HANDLE)_get_osfhandle(fDest->handle() );

    LARGE_INTEGER  file_size;
    file_size.QuadPart = fsize;

    bool success = false;
    if( SetFilePointerEx( HFile, file_size, NULL, FILE_BEGIN ) )
        success = SetEndOfFile ( HFile );

    file_size.QuadPart = 0;
    SetFilePointerEx(HFile,file_size,NULL,FILE_BEGIN);
    if( !success )
    {

        if( fDest->size() == fsize )
            success = true;
    }
    //success = fDest->resize(fsize); <-- too slow

    return success;
}

/** should retry copying a file whenever the location was lost (e.g. because of a disconnect) **/
bool mFile::ShouldRetry()
{
    if( CopyThreadInfo->copy_type == LOCAL_NETWORK || CopyThreadInfo->copy_type == FROM_INET )
    {
	if( fDest->error() == QFile::TimeOutError || fDest->error() == QFile::OpenError )
	{
	    QFileInfo f(QString::fromStdWString(fDestName));
	    QDir d(f.path());
	    derror = fDest->errorString().toStdWString();
	    if( !d.exists() )
	    {
		retry = true;
		return true;
	    }
	    else
	    {
		retry = false;
		return false;
	    }
	}
	if(fSource->error() == QFile::TimeOutError || fSource->error() == QFile::OpenError )
	{
	    QFileInfo f(QString::fromStdWString(fSourceName));
	    QDir d(f.path());
	    serror = fSource->errorString().toStdWString();
	    if( !d.exists() )
	    {
		retry = true;
		return true;
	    }
	    else
	    {
		retry = false;
		return false;
	    }
	}
    }
    return false;
}

bool mFile::mCopyFile(QFile *fSource, QFile *fDest)
{
    QTime savestatetime;
    savestatetime.start();
    size_t copysize = buffersize;
    copy_started = true;
    derror=L"";
    serror=L"";

    while( !fSource->atEnd() && !fSource->error() && !CopyThreadInfo->TestDestroy() )
    {
        QTimeWatch readtime,writetime;
        readtime.start();
        qint64 readbit = fSource->read(buffer,copysize);
        if( readbit < 1 )
        {
            serror = fSource->errorString().toStdWString();
            return false;
        }


        writetime.start();
        qint64 writebit = fDest->write(buffer,readbit);

	if( CopyThreadInfo->copy_type == LOCAL_NETWORK || CopyThreadInfo->copy_type == FROM_INET )
            fDest->flush();

        writetime.pause();

        if( writebit != readbit )
        {
            derror = fDest->errorString().toStdWString();
            return false;
        }

        mCopiedSize += writebit;
        CopyThreadInfo->addCopiedSize(writebit);
        CopyThreadInfo->addSizeDone(writebit);

        int time = writetime.time();
        if( time < readtime.time())
            time = readtime.time();

        if(!fSource->atEnd())
        {
            size_t bfsize = Calc_BfSize(time);
            if( bfsize > 0)
            {
                copysize = bfsize;
                if( bfsize > buffersize )
                {
                    buffersize = bfsize;
                    buffer = (char*) realloc(buffer,buffersize);
                }
            }
        }

        if( savestatetime.elapsed() > 10*1000 )
        {
            if( fDest->flush() )
            {
                SaveCopiedSize();
            }else
            {
                derror = fDest->errorString().toStdWString();
                return false;
            }
            savestatetime.restart();
        }
    }
    if(CopyThreadInfo->TestDestroy())
    {
        SaveCopiedSize();
        return true;
    }
    copied = true;
    return true;
}

bool mFile::StartCopyFile()
{
    bool try_again=true;
    average_speed = buffersize;
    if(fDest==NULL)
        fDest = new QFile(QString::fromStdWString(fDestName));
    if(fSource==NULL)
        fSource = new QFile(QString::fromStdWString(fSourceName));

    while(1)
    {
        if(CopyThreadInfo->TestDestroy())
        {
            SaveCopiedSize();
            break;
        }


        fSource->open(QFile::ReadOnly);
        if( !fSource->isOpen() )
        {
            serror = fSource->errorString().toStdWString();
            if( ShouldRetry() )
            {
                QThread::msleep(500);
                continue;
            }else if(try_again)
            {
                try_again = false;
                continue;
            }
            delete fSource;
            fSource = NULL;
            return false;
        }


        if((IsFirstInQueue || retry) && fDest->exists())
            fDest->open(QFile::ReadWrite);
        else
        {
            if( fDest->open(QFile::WriteOnly) )
            {
                FILETIME creation,lastAccess,lastModified;
                GetFileTime((HANDLE)_get_osfhandle(fSource->handle()),&creation,&lastAccess,&lastModified);
                SetFileTime((HANDLE)_get_osfhandle(fDest->handle()),&creation,&lastAccess,&lastModified);
            }
        }
        if( !fDest->isOpen() )
        {
            derror = fDest->errorString().toStdWString();
            if( ShouldRetry() )
            {
                QThread::msleep(500);
                continue;
            }else if(try_again)
            {
                try_again = false;
                continue;
            }
            delete fDest;
            fDest = NULL;
            return false;
        }
        try_again = true;

        if( fsize == 0 )
        {
            fSource->close();
            fDest->close();
            return true;
        }


        if( IsFirstInQueue || retry)
        {
            double offset = GetCopiedSize();
            fSource->seek( offset );
            fDest->seek( offset );
            if( retry )
            {
                CopyThreadInfo->addSizeDone(-mCopiedSize);
                CopyThreadInfo->addSizeDone(offset);
            }
            mCopiedSize = offset;
        }
        else
        {
            if( !SetFileSize() )
            {
                delete fSource;
                delete fDest;
                fSource = NULL;
                fDest = NULL;
                return false;
            }

        }

        bool ok = mCopyFile(fSource,fDest);
        fSource->close();
        fDest->close();

        if(!ok)
        {
	    QThread::msleep(500);
	    if( CopyThreadInfo->copy_type == LOCAL_NETWORK || CopyThreadInfo->copy_type == FROM_INET )
	    {
		continue;
	    }
        }
        return ok;
    }
    return false;
}


bool mFile::DeleteDest()
{
    QString file = QString::fromStdWString(fDestName);
    if(file.indexOf(QString("/")) == -1)
    {
        return true;
    }
    if(!QFile::exists(file))
        return true;

    CopyThreadInfo->SetCurrentFile(file.afterLast('/'));

    while(1)
    {
        if ( !QFile::remove( file ))
        {
            int res;
            CopyThreadInfo->message(QString("Can't remove file: ") + file + QString("\n retry?"),&res,QMessageBox::No|QMessageBox::Cancel|QMessageBox::Yes);
            if ( res == QMessageBox::No )
            {
                break;
            }
            if ( res == QMessageBox::Cancel )
                return false;
        }else
            break;


        if ( CopyThreadInfo->TestDestroy() )
            return false;
    }
    CopyThreadInfo->addFileCount(-1);

    return true;
}

bool mFile::DeleteSource()
{
    QString file = QString::fromStdWString(fSourceName);

    CopyThreadInfo->SetCurrentFile(file.afterLast('/'));
    while(1)
    {
        if ( !QFile::remove( file ) )
        {
            if( !QFile::exists(file) )
                break;

            int res;
            CopyThreadInfo->message(QString("Can't remove file: ") + file + QString("\n retry?"),&res,QMessageBox::No|QMessageBox::Cancel|QMessageBox::Yes);
            if ( res == QMessageBox::No )
            {
                break;
            }
            if ( res == QMessageBox::Cancel )
                return false;
        }else
            break;

        if ( CopyThreadInfo->TestDestroy() )
            return false;
    }
    CopyThreadInfo->addFileCount(-1);
    return true;
}
