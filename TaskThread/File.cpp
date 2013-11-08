

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
#define MIN_SIZE  32*1024


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

    if(fDestName.length() == 0){
        qWarning()<<"fdestName empty for : " + fSourceName;
        return;
    }

    fsize = fileinfo.size();
    qDebug()<<"size of "<<fileinfo.absolutePath()<<": "<<fsize;
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

        sourceModifiedStr = sourceModified.toString().toLocal8Bit();
        targetModifiedStr = destModified.toString().toLocal8Bit();

        targetSize = destfileinfo.size();
        if(targetSize<fsize)
            destSize = SMALLER;
        if(targetSize>fsize)
            destSize = BIGGER;
        if(targetSize==fsize)
            destSize = SAME;
    }
}

QString File::getSourceModifiedDate()
{
    return sourceModifiedStr;
}

QString File::getTargetModifiedDate()
{
    return targetModifiedStr;
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
    fDestName = name.toUtf8();
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
    size_t bfSize = this->buffersize;
    QByteArray array;

    while(!destFile.atEnd())
    {
        QTime watch;
        watch.start();
        array = destFile.read(bfSize);
        int time = watch.elapsed();

        if(!destFile.atEnd())
            bfSize = this->computeBufferSize(time);

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
        qWarning()<<"File: checksum failed";

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
        fDestName = getSourceName().afterLast("/").toUtf8();
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
        fDestName = new_name.toUtf8();
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
        fDestName = new_name.toUtf8();

        ignore(false);
        replace(false);

        copyhandle = COPY_RENAME;
        if(destExist)
            parentFolder->addSizeExistingFile(-fsize);
    }
    return true;
}

size_t File::computeBufferSize(int time)
{
    double p = 0;

    if ( time == 0 )
    {
        if(averageSpeed*2>MAX_SIZE)
            return MAX_SIZE;
        return 2*averageSpeed;
    }

    //different timings for copying over network
    bool reduce_size = false;
    bool increase_size = false;
    if( copyThreadInfo->getCopyType() == TaskListHandler::LOCAL_NETWORK
            || copyThreadInfo->getCopyType() == TaskListHandler::FROM_INET )
    {
        if ( time < 1500 ){
            increase_size = true;
        }
        if ( time > 3500 ){
            reduce_size = true;
        }
    }else{

    }

    if ( increase_size )
    {
        double sp = 2*averageSpeed;
        if( sp > MAX_SIZE )
            return MAX_SIZE;
        p = sp;
        averageSpeed = sp;
    }

    if ( reduce_size )
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
        qDebug()<<"File: failed to get file handle";
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
    if( copyThreadInfo->getCopyType() == TaskListHandler::LOCAL_NETWORK
            || copyThreadInfo->getCopyType() == TaskListHandler::FROM_INET )
    {
        if( fDest->error() == QFile::TimeOutError || fDest->error() == QFile::OpenError )
        {
            QFileInfo f(fDestName);
            QDir d(f.path());
            derror = fDest->errorString().toUtf8();
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
        if(fSource->error() == QFile::TimeOutError
                || fSource->error() == QFile::OpenError )
        {
            QFileInfo f(fSourceName);
            QDir d(f.path());
            serror = fSource->errorString().toUtf8();
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


void File::handleZeroSizeFile()
{
    DWORD attr = GetFileAttributes(QString(fSourceName).toStdWString().c_str());
    QString binary;binary.setNum(attr,2);
    qDebug()<<"File: "<<fSourceName;
    qDebug()<<"File: attributes: "<<binary;
    if( attr!= INVALID_FILE_ATTRIBUTES){
            SetFileAttributes(QString(fDestName).toStdWString().c_str(),attr);
    }else{
        qCritical()<<"File: Invalid attributes for "<<fDestName;
    }

    copied = true;
}

bool File::prepareCopy()
{
    bool retryIfFailed=true;
    averageSpeed = buffersize;


    if(fDest)delete fDest;
    if(fSource)delete fSource;
    fDest = new QFile(fDestName);
    fSource = new QFile(fSourceName);

    if( fDest==fSource ){

        derror = "You can't overwrite a file with itself!";
        return false;
    }

    while(1)
    {
        //close for retrying, prevents "already open messsage in debug window"
        fSource->close();
        fDest->close();
        qDebug()<<"File: preparing...";
        if(copyThreadInfo->testDestroy())
        {
            saveCopiedSize();
            break;
        }

        copyStarted = true;
        fSource->open(QFile::ReadOnly);
        if( !fSource->isOpen() )
        {
            serror = fSource->errorString().toUtf8();
            goto failed;
        }

        //delete destination file if it should be replaced
        if( fDest->exists() && copyhandle==COPY_REPLACE ){

            //ignore permissions, also read-only, seems to be default on windows
            if( !NcSettings::getValue<bool>(NcSettings::CHECK_PERMISSIONS) ){

                if( !fDest->isWritable() ){
                    fDest->setPermissions(QFile::WriteUser);
                }
                if( !fDest->remove() ){
                    qDebug()<<"File: "<<fDestName;
                    qDebug()<<"File: failed to remove -> "<<fDest->errorString();;
                }
            }
        }

        if( ( isFirst() || shouldRetry() ) && fDest->exists())
        {
            //continue where we left
            fDest->open(QFile::ReadWrite);
        }
        else
        {
            //create new file
            if( fDest->open(QFile::WriteOnly) )
            {
                FILETIME creation,lastAccess,lastModified;
                GetFileTime((HANDLE)_get_osfhandle(fSource->handle()),&creation,&lastAccess,&lastModified);
                SetFileTime((HANDLE)_get_osfhandle(fDest->handle()),&creation,&lastAccess,&lastModified);
            }
        }
        if( !fDest->isOpen() )
        {
            derror = fDest->errorString().toUtf8();
            goto failed;
        }
        retryIfFailed = true;

        if( fsize == 0 )
        {
            handleZeroSizeFile();
            return true;
        }


        if( !setFileSize() )
        {
            return false;
        }

        if( isFirst() || shouldRetry() )
        {
            double offset = getSavedCopiedSize();
            fSource->seek( offset );
            fDest->seek( offset );
            if( shouldRetry() )
            {
                copyThreadInfo->addSizeDone(-copiedSize);
                copyThreadInfo->addSizeDone(offset);
            }
            copiedSize = offset;
        }


        return true;

        failed:

        qDebug()<<"File: something failed";
        qDebug()<<"File: "<<derror;

        if( checkShouldRetry() )
        {
            QThread::msleep(500);
            continue;
        }else if(retryIfFailed)
        {
            retryIfFailed = false;
            continue;
        }
            return false;
        }
}

bool File::startCopyFile()
{
    bool ok = false;
    while(1)
    {

        qDebug("File: preparing copy");
        ok = prepareCopy();
        if(!ok)
            break;

        qDebug("File: copying");
        ok = copyFile();

        if(!ok)
        {
            QThread::msleep(500);
            if( copyThreadInfo->getCopyType() == TaskListHandler::LOCAL_NETWORK
                || copyThreadInfo->getCopyType() == TaskListHandler::FROM_INET )
            {
                QThread::msleep(500);
                qDebug("File: retrying copy (is on network)");
                continue;
            }
        }

        DWORD attr = GetFileAttributes(QString(fSourceName).toStdWString().c_str());
        QString binary;binary.setNum(attr,2);
        qDebug()<<"File: "<<fSourceName;
        qDebug()<<"File: attributes: "<<binary;
        if( attr!= INVALID_FILE_ATTRIBUTES){
                SetFileAttributes(QString(fDestName).toStdWString().c_str(),attr);
        }else{
            qCritical()<<"File: Invalid attributes for "<<fDestName;
        }

        //if we got till here break
        break;
    }
    fDest->flush();
    delete fDest;
    delete fSource;
    fDest = NULL;
    fSource = NULL;
    return ok;
}


bool File::copyFile()
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
            serror = fSource->errorString().toUtf8();
            return false;
        }

        readtime.pause();

        this->sourceCheckSum = myChecksum(buffer,readbyte,~this->sourceCheckSum);


        writetime.start();
        qint64 writebit = fDest->write(buffer,readbyte);

        if( copyThreadInfo->getCopyType() == TaskListHandler::LOCAL_NETWORK
                || copyThreadInfo->getCopyType() == TaskListHandler::FROM_INET ){

            fDest->flush();
        }

        writetime.pause();

        if( writebit != readbyte )
        {
            derror = fDest->errorString().toUtf8();
            return false;
        }

        copiedSize += writebit;
        copyThreadInfo->addCopiedSize(writebit);
        copyThreadInfo->addSizeDone(writebit);

        int time = writetime.time();
        if( time < readtime.time())
            time = readtime.time();

        if(writetime.time()<=1 || readtime.time()<=1 ){
            time = 0;
        }

        if(!fSource->atEnd() && time!=0 )
        {
            size_t bfsize = computeBufferSize(time);
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
                derror = fDest->errorString().toUtf8();
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

bool File::deleteDest()
{

    if(copyThreadInfo->testDestroy())
        return false;


    if(!this->hasCopyStarted())
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
            copyThreadInfo->sendMessage(QWidget::tr("Can't remove file: ") + file + QWidget::tr("\n retry?"),
                                        &res,
                                        QMessageBox::No|QMessageBox::Cancel|QMessageBox::Yes);
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

    if( this->copyhandle==COPY_IGNORE ||
            this->skip ||
            !this->hasCopyStarted()||
            !this->wasCopied()){

        return true;
    }

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
            copyThreadInfo->sendMessage(QWidget::tr("Can't remove file: ") + file + QWidget::tr("\n retry?"),
                                        &res,
                                        QMessageBox::No|QMessageBox::Cancel|QMessageBox::Yes);
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
