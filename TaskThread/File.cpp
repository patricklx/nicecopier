

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
#include <QCryptographicHash>

#include <QTreeWidgetItem>
#include <QFileIconProvider>
#include "ReadableSize.h"
#include "ncsettings.h"

#include "File.h"
#include <windows.h>
#include <io.h>
#define MAX_SIZE  4 * 1024 * 1024
#define MIN_SIZE  64*1024


int interval_pause=0;

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

static const quint16 crc_tbl[16] = {
    0x0000, 0x1081, 0x2102, 0x3183,
    0x4204, 0x5285, 0x6306, 0x7387,
    0x8408, 0x9489, 0xa50a, 0xb58b,
    0xc60c, 0xd68d, 0xe70e, 0xf78f
};

quint16 myChecksum(const char *data, uint len,register quint16 crc = 0xffff)
{
    uchar c;
    const uchar *p = reinterpret_cast<const uchar *>(data);
    while (len--) {
        c = *p++;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
        c >>= 4;
        crc = ((crc >> 4) & 0x0fff) ^ crc_tbl[((crc ^ c) & 15)];
    }
    return ~crc & 0xffff;
}


File::File(TaskThread* thread,Folder* folder,QFileInfo fileinfo,bool check_exists)
{
    initialize(thread,folder,fileinfo,check_exists);
}


File::File(TaskThread *copyinfo, Folder *folder, QXmlStreamAttributes attributes)
{
    initialize(copyinfo,folder);
    fDestName = attributes.value("fDestName").toString().toUtf8();
    fSourceName = attributes.value("fSourceName").toString().toUtf8();
    fsize = attributes.value("fsize").toString().toDouble();
    copyhandle = (File::CopyHandle) QVariant(attributes.value("copyhandle").toString()).toInt();
    copyStarted = QVariant(attributes.value("copy_started").toString()).toBool();
    copied = QVariant(attributes.value("copied").toString()).toBool();
    destExist = QVariant(attributes.value("exist").toString()).toBool();
    destAge = (File::AgeCompare) attributes.value("destAge").toString().toInt();
    targetSize = attributes.value("targetSize").toString().toDouble();
}

File::~File()
{
    if( fDest != NULL)
        delete fDest;
    if( fSource != NULL)
        delete fSource;
    delete treeItem;
}

void File::initialize(TaskThread *copyinfo, Folder *folder, QFileInfo fileinfo, bool check_exists)
{
    buffer = NULL;
    fDest = NULL;
    fSource = NULL;
    firstInQueue = false;
    buffersize = 0;
    copiedSize = 0;
    averageSpeed = 64*1024;
    retry = false;
    copyhandle = COPY_NOT_SET;
    skip = false;
    copyStarted = false;
    copyThreadInfo = copyinfo;
    parentFolder = folder;
    destExist = false;
    copied = false;
    checkSumStatus = NOT_TESTED;
    targetSize = 0;
    sourceCheckSum = 0;

    fDestName = fileinfo.fileName().toUtf8();
    fSourceName = fileinfo.filePath().toUtf8();

    treeItem = new TreeItem(*this,this->parentFolder->getTreeItem());
    treeItem->setRowIndex(parentFolder->getSubFolderList().count()+parentFolder->getFileList().count());

    if(fDestName.length() == 0)
        return;

    fsize = fileinfo.size();
    copyinfo->addFileCount(1);
    copyinfo->addTotalSize(fsize);

    QString name = parentFolder->getDestPath() +  fDestName ;
    QFileInfo destfileinfo(name);

    if(check_exists && exists(true))
    {
        destExist = true;
        QDateTime sourceModified = fileinfo.lastModified();
        QDateTime destModified = destfileinfo.lastModified();

        if(destModified<sourceModified)
            destAge = OLDER;

        if(destModified>sourceModified)
            destAge = NEWER;

        if(destModified==sourceModified)
            destAge = EQUAL;

        targetSize = destfileinfo.size();
        destSize = (targetSize==fsize)?SAME:DIFFERENT;
    }
}

double File::getTargetSize()
{
    return targetSize;
}

double File::getSize()
{
    return fsize;
}

void File::setSpeed(int speed)
{
    interval_pause = (10.0-speed)/10.0*1000;
}

TreeItem* File::getTreeItem()
{
    return treeItem;
}

Folder* File::getParentFolder()
{
    return parentFolder;
}

File::CopyHandle File::getCopyHandle()
{
    return copyhandle;
}

void File::setCopyHandle(File::CopyHandle handle)
{
    copyhandle = handle;
}

void File::setDestinationName(QString name)
{
    fDestName = name.toAscii();
}

bool File::verifyChecksum()
{
    QFile destFile(fDestName);

    copyThreadInfo->setCurrentFile(QStringExt(fDestName).afterLast("/"));
    copyThreadInfo->setCurrentDirectory(QStringExt(fSourceName).beforeLast("/"),QStringExt(fDestName).beforeLast("/"));
    if(!destFile.open(QFile::ReadOnly))
    {
        qWarning("failed to open target file to check hash");
        return false;
    }

    qint16 checkSum = 0;
    QByteArray array;
    while(!destFile.atEnd())
    {
        array = destFile.read(4*1024*1024);
        checkSum = myChecksum(array,array.length(),~checkSum);

        copyThreadInfo->addSizeDone(array.length());
        copyThreadInfo->addCopiedSize(array.length());

        if( copyThreadInfo->testDestroy() )
            break;
    }

    copyThreadInfo->addFileCount(-1);

    bool result = checkSum==sourceCheckSum;

    if( result == false ){

        this->derror = "checksum test failed!";
        this->checkSumStatus = FAILED;
        qDebug()<<"checksum failed";

    }else{
        this->checkSumStatus = PASSED;
    }

    return result;
}

QStringExt File::getDestinationName()
{
    return fDestName;
}

QStringExt File::getSourceName()
{
    return fSourceName;
}

QString File::getSourceError()
{
    return serror;
}

QString File::getDestinationError()
{
    return derror;
}

void File::setBuffer(char *buffer, int size)
{
    this->buffer = buffer;
    this->buffersize = size;
}

int File::getBufferSize()
{
    return buffersize;
}

bool File::exists(bool recheck)
{
    if(!recheck){
        return destExist;
    }
    QString path = this->copyStarted?"":parentFolder->getDestPath();
    QString name = path + fDestName;
    return QFile::exists(name);
}

void File::replace(bool replace)
{
    if(parentFolder->isReplace() || !destExist)
        return;

    if(replace && copyhandle!=File::COPY_REPLACE)
    {
        ignore(false);
        unrename();

        parentFolder->addSizeExistingFile(-fsize);
        parentFolder->addSizeReplace(fsize);
        copyhandle = File::COPY_REPLACE;
    }
    else if(!replace && copyhandle==File::COPY_REPLACE)
    {
        parentFolder->addSizeExistingFile(+fsize);
        parentFolder->addSizeReplace(-fsize);
        copyhandle = File::COPY_NOT_SET;
    }
}

bool File::shouldRetry()
{
    return retry;
}


void File::unrename()
{
    if(copyhandle==COPY_RENAME)
    {
        fDestName = getSourceName().afterLast("/").toAscii();
        copyhandle = COPY_NOT_SET;

        if(destExist)
            parentFolder->addSizeExistingFile(+fsize);
    }
}

void File::ignoreExisting(bool m_ignore)
{
    if(destExist)
        ignore(m_ignore);
}

void File::ignore(bool m_ignore)
{
    if(parentFolder->isIgnore() || (parentFolder->isIgnoreExisting() && destExist) )
        return;

    if(m_ignore && copyhandle!=COPY_IGNORE)
    {
        unrename();
        replace(false);
        copyhandle = COPY_IGNORE;
        if(destExist)
            parentFolder->addSizeExistingFile(-fsize);
        parentFolder->addSize(-this->fsize);
    }
    else if(!m_ignore &&  copyhandle==COPY_IGNORE)
    {
        copyhandle = COPY_NOT_SET;
        if(destExist)
            parentFolder->addSizeExistingFile(+fsize);
        parentFolder->addSize(fsize);
    }
}

bool File::rename(QStringExt new_name)
{
    if(!new_name.isEmpty())
    {
        QByteArray temp = fDestName;
        fDestName = new_name.toAscii();
        if(exists(true))
        {
            fDestName = temp;
            return false;
        }
        ignore(false);
        replace(false);
        copyhandle = COPY_RENAME;
        if(destExist)
            parentFolder->addSizeExistingFile(-fsize);

        return true;
    }

    if( copyhandle != COPY_RENAME )
    {
        if(new_name.isEmpty())
            new_name = fDestName;


        QString path = parentFolder->getDestPath();
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
        fDestName = new_name.toAscii();

        ignore(false);
        replace(false);

        copyhandle = COPY_RENAME;
        if(destExist)
            parentFolder->addSizeExistingFile(-fsize);
    }
    return true;
}

size_t File::computeBfSize(int time)
{
    double p = 0;

    if ( time == 0 )
    {
        if(averageSpeed*2>MAX_SIZE)
            return MAX_SIZE;
        return 2*averageSpeed;
    }

    if ( time < 400 )
    {
        double sp = 2*averageSpeed;
        if( sp > MAX_SIZE )
            return MAX_SIZE;
        p = sp;
        averageSpeed = sp;
    }

    if ( time > 1500 )
    {
        double sp =  averageSpeed/2;
        if ( sp < MIN_SIZE  )
            return MIN_SIZE;
        p = sp;
        averageSpeed = sp;
    }

    return p;
}

void File::saveCopiedSize()
{
    QString taskfile;
    taskfile.sprintf( "Tasks/Task%i.TaskLastFile",  copyThreadInfo->getTaskId() );
    QFile file(taskfile);
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ))
        return;

    QString content;
    content.setNum(copiedSize,'g',100);

    QTextStream stream(&file);
    stream << fSourceName << "\n";
    stream << content << "\n";
    stream << this->sourceCheckSum;
}

double File::getCopiedSize()
{
    return this->copiedSize;
}

File::ChecksumStatus File::getChecksumStatus()
{
    return this->checkSumStatus;
}

double File::getSavedCopiedSize()
{
    double size;
    QString taskfile;
    taskfile.sprintf( "Tasks/Task%i.TaskLastFile", copyThreadInfo->getTaskId() );
    QFile file(taskfile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text ))
        return 0;

    QTextStream in(&file);
    in.readLine();
    in >> size;
    in >> this->sourceCheckSum;

    return size;
}


bool File::setFileSize()
{
    if(fDest->size() == fsize)
        return true;
    HANDLE HFile = (HANDLE)_get_osfhandle(fDest->handle() );
    if(HFile==NULL)
    {
        qDebug("failed to get file handle");
        return false;
    }

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
        {
            success = true;
        }else
        {
            derror = "Failed to reserve disk space!";
            if(fsize >= 4.0*1073741824 )//4 Gigibiytes
                derror += "Check if the filesystem supports files bigger than 4 Gigabytes";
        }
    }
    //success = fDest->resize(fsize); <-- too slow

    return success;
}

/** should retry copying a file whenever the location was lost (e.g. because of a disconnect) **/
bool File::checkShouldRetry()
{
    if( copyThreadInfo->getCopyType() == TaskListHandler::LOCAL_NETWORK || copyThreadInfo->getCopyType() == TaskListHandler::FROM_INET )
    {
        if( fDest->error() == QFile::TimeOutError || fDest->error() == QFile::OpenError )
        {
            QFileInfo f(fDestName);
            QDir d(f.path());
            derror = fDest->errorString().toAscii();
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
            QFileInfo f(fSourceName);
            QDir d(f.path());
            serror = fSource->errorString().toAscii();
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

bool File::shouldSkip()
{
    return skip;
}

void File::setRetry(bool retry)
{
    this->retry = retry;
}

void File::setSkip(bool skip)
{
    this->skip = skip;
}

bool File::isRenamed()
{
    return getSourceName().afterLast("/") != getDestinationName().afterLast("/");
}

bool File::wasCopied()
{
    return copied;
}

bool File::hasCopyStarted()
{
    return copyStarted;
}

File::AgeCompare File::compareAge()
{
    return destAge;
}

File::SizeCompare File::compareSize()
{
    return destSize;
}

bool File::isFirst()
{
    return firstInQueue;
}

void File::setFirst()
{
    firstInQueue = true;
}


bool File::startCopyFile()
{
    bool try_again=true;
    averageSpeed = buffersize;

    QFile flDest(fDestName);
    QFile flSource(fSourceName);

    fDest = &flDest;
    fSource = &flSource;

    while(1)
    {
        if(copyThreadInfo->testDestroy())
        {
            saveCopiedSize();
            break;
        }

        copyStarted = true;
        flSource.open(QFile::ReadOnly);
        if( !flSource.isOpen() )
        {
            serror = flSource.errorString().toAscii();
            if( checkShouldRetry() )
            {
                QThread::msleep(500);
                continue;
            }else if(try_again)
            {
                try_again = false;
                continue;
            }
            fDest = NULL;
            fSource = NULL;
            return false;
        }


        if((firstInQueue || retry) && flDest.exists())
            flDest.open(QFile::ReadWrite);
        else
        {
            if( flDest.open(QFile::WriteOnly) )
            {
                FILETIME creation,lastAccess,lastModified;
                GetFileTime((HANDLE)_get_osfhandle(fSource->handle()),&creation,&lastAccess,&lastModified);
                SetFileTime((HANDLE)_get_osfhandle(fDest->handle()),&creation,&lastAccess,&lastModified);
            }
        }
        if( !flDest.isOpen() )
        {
            derror = flDest.errorString().toAscii();
            if( checkShouldRetry() )
            {
                QThread::msleep(500);
                continue;
            }else if(try_again)
            {
                try_again = false;
                continue;
            }
            fDest = NULL;
            fSource = NULL;
            return false;
        }
        try_again = true;

        if( fsize == 0 )
        {
            flSource.close();
            flDest.close();
            fDest = NULL;
            fSource = NULL;
            DWORD attr = GetFileAttributes(QString(fSourceName).toStdWString().c_str());
            SetFileAttributes(QString(fDestName).toStdWString().c_str(),attr);
            return true;
        }


        if( !setFileSize() )
        {
            fDest = NULL;
            fSource = NULL;
            return false;
        }

        if( firstInQueue || retry)
        {
            double offset = getSavedCopiedSize();
            flSource.seek( offset );
            flDest.seek( offset );
            if( retry )
            {
                copyThreadInfo->addSizeDone(-copiedSize);
                copyThreadInfo->addSizeDone(offset);
            }
            copiedSize = offset;
        }

        bool ok = copyFile(&flSource,&flDest);
        flSource.close();
        flDest.close();

        if(!ok)
        {
            QThread::msleep(500);
            if( copyThreadInfo->getCopyType() == TaskListHandler::LOCAL_NETWORK
                || copyThreadInfo->getCopyType() == TaskListHandler::FROM_INET )
            {
                continue;
            }
        }
        fDest = NULL;
        fSource = NULL;
        DWORD attr = GetFileAttributes(QString(fSourceName).toStdWString().c_str());
        SetFileAttributes(QString(fDestName).toStdWString().c_str(),attr);
        return ok;
    }
    fDest = NULL;
    fSource = NULL;
    return false;
}


bool File::copyFile(QFile *fSource, QFile *fDest)
{
    QTime savestatetime;
    savestatetime.start();
    size_t copysize = buffersize;
    copyStarted = true;
    derror="";
    serror="";

    while( !fSource->atEnd() && !fSource->error() && !copyThreadInfo->testDestroy() )
    {
        QTimeWatch readtime,writetime;
        readtime.start();
        qint64 readbyte = fSource->read(buffer,copysize);
        if( readbyte < 1 )
        {
            serror = fSource->errorString().toAscii();
            return false;
        }

        this->sourceCheckSum = myChecksum(buffer,readbyte,~this->sourceCheckSum);


        writetime.start();
        qint64 writebit = fDest->write(buffer,readbyte);

        if( copyThreadInfo->getCopyType() == TaskListHandler::LOCAL_NETWORK || copyThreadInfo->getCopyType() == TaskListHandler::FROM_INET )
            fDest->flush();

        writetime.pause();

        if( writebit != readbyte )
        {
            derror = fDest->errorString().toAscii();
            return false;
        }

        copiedSize += writebit;
        copyThreadInfo->addCopiedSize(writebit);
        copyThreadInfo->addSizeDone(writebit);

        int time = writetime.time();
        if( time < readtime.time())
            time = readtime.time();

        if(!fSource->atEnd())
        {
            size_t bfsize = computeBfSize(time);
            if( bfsize > 0)
            {
                copysize = bfsize;
                buffersize = bfsize;
            }
        }

        if( savestatetime.elapsed() > 10*1000 )
        {
            if( fDest->flush() )
            {
                saveCopiedSize();
            }else
            {
                derror = fDest->errorString().toAscii();
                return false;
            }
            savestatetime.restart();
        }
        if(interval_pause > 0)
        {
            QThread::msleep(interval_pause);
        }
    }
    if(copyThreadInfo->testDestroy())
    {
        saveCopiedSize();
        return true;
    }
    copied = true;
    return true;
}

/*
bool mFile::StartCopyFile()
{
    bool try_again=true;
    average_speed = buffersize;
    if(fDest==NULL)
        fDest = new QFile(QString(fDestName));
    if(fSource==NULL)
        fSource = new QFile(QString(fSourceName));

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
            serror = fSource->errorString().toAscii();
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
            delete fDest;
            fSource = NULL;
            fDest = NULL;
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
            derror = fDest->errorString().toAscii();
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
            delete fDest;
            fSource = NULL;
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
        delete fSource;
        delete fDest;
        fSource = NULL;
        fDest = NULL;
        return ok;
    }
    delete fSource;
    delete fDest;
    fSource = NULL;
    fDest = NULL;
    return false;
}*/

bool File::deleteDest()
{
    if(copyThreadInfo->testDestroy())
        return false;

    if(!copyStarted)
        return true;

    QStringExt file = fDestName;
    if(file.indexOf("/") == -1)
    {
        return true;
    }
    if(!QFile::exists(file))
        return true;

    SetFileAttributes(file.toStdWString().c_str(),FILE_ATTRIBUTE_NORMAL);

    copyThreadInfo->setCurrentFile(file.afterLast('/'));
    copyThreadInfo->setCurrentDirectory("",file.beforeLast("/"));

    while(1)
    {
        if ( !QFile::remove( file ) && QFile::exists(file) )
        {
            int res;
            copyThreadInfo->sendMessage(QString("Can't remove file: ") + file + QString("\n retry?"),&res,QMessageBox::No|QMessageBox::Cancel|QMessageBox::Yes);
            if ( res == QMessageBox::No )
            {
                break;
            }
            if ( res == QMessageBox::Cancel )
                return false;
        }else
            break;


        if ( copyThreadInfo->testDestroy() )
            return false;
    }
    copyThreadInfo->addFileCount(-1);

    return true;
}

bool File::deleteSource()
{
    if(copyThreadInfo->testDestroy())
        return false;



    QStringExt file = QString(fSourceName);
    SetFileAttributes(file.toStdWString().c_str(),FILE_ATTRIBUTE_NORMAL);

    copyThreadInfo->setCurrentFile(file.afterLast('/'));
    copyThreadInfo->setCurrentDirectory(file.beforeLast("/"),"");
    while(1)
    {
        if ( !QFile::remove( file ) )
        {
            if( !QFile::exists(file) )
                break;

            int res;
            copyThreadInfo->sendMessage(QString("Can't remove file: ") + file + QString("\n retry?"),&res,QMessageBox::No|QMessageBox::Cancel|QMessageBox::Yes);
            if ( res == QMessageBox::No )
            {
                break;
            }
            if ( res == QMessageBox::Cancel )
                return false;
        }else
            break;

        if ( copyThreadInfo->testDestroy() )
            return false;
    }
    copyThreadInfo->addFileCount(-1);
    return true;
}
