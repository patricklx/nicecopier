#include "Folder.h"
#include <QDir>
#include <QFileIconProvider>
#include <qdatetime.h>
#include <QSettings>
#include <QMessageBox>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>
#include <windows.h>

#include "taskthread.h"
#include "ReadableSize.h"


Folder::Folder(TaskThread *thread,Folder *parent, QFileInfo folderinfo,bool check_exist)
{
    initialize(thread,parent,folderinfo,check_exist);
}

Folder::Folder(TaskThread *copyinfo, QFileInfo source, QFileInfo destination)
{
    qDebug()<<"Folder: source recieved: "<< source.path();
    qDebug()<<"Folder: target recieved: "<<destination.path();
    initialize(copyinfo,NULL,source);
    destPath = QStringExt(destination.path()).toUtf8();
    QStringExt destinationPath = QStringExt(destination.path());

    QStringList list = QString(sourcePath).split('/');

    sourceName = list.takeLast().toUtf8();
    sourcePath = QString(list.isEmpty()?(""):list.join("/")+"/").toUtf8();

    list = QString(destPath).split('/');

    destName = list.takeLast().toUtf8();
    destPath = QString(list.isEmpty()?"":list.join("/")+"/").toUtf8();

    if(exists())
    {
        destExist = true;
    }

    qDebug()<<"Folder: source: "<<sourcePath;
    qDebug()<<"Folder: sourceName: "<<sourceName;
    qDebug()<<"Folder: destination: "<<destPath;
    qDebug()<<"Folder: destinationName: "<<destName;
}

Folder::Folder(TaskThread *copyinfo, Folder *parent, QXmlStreamAttributes attributes)
{
    initialize(copyinfo,parent);
    sourceName = attributes.value("source_name").toString().toUtf8();
    destName = attributes.value("dest_name").toString().toUtf8();
    copyhandle = (Folder::CopyHandle) QVariant(attributes.value("copyhandle").toString()).toInt();
    sourcePath = attributes.value("source_path").toString().toUtf8();
    destPath = attributes.value("dest_path").toString().toUtf8();
    if(exists())
    {
        destExist = true;
    }
}

Folder::~Folder()
{
    while( !fileList.isEmpty() )
    {
        File *file = fileList.takeFirst();
        delete file;
    }

    while( !subfolderList.isEmpty() )
    {
        Folder *folder = subfolderList.takeFirst();
        delete folder;
    }
    if(treeItem)
        delete treeItem;
}


void Folder::initialize(TaskThread *copyinfo, Folder *parent, QFileInfo folderinfo, bool check_exist)
{
    filesCopied = false;
    fsize = 0;
    parentFolder = parent;
    copyStarted = false;
    copyThreadInfo = copyinfo;
    destExist = false;
    copyhandle = COPY_NOT_SET;
    fsizeExisting  = 0;
    fsizeReplace = 0;


    sourceName = folderinfo.fileName().toUtf8();
    destName = folderinfo.fileName().toUtf8();
    sourcePath = folderinfo.path().toUtf8();

    treeItem = NULL;
    if(this->parentFolder)
    {
        treeItem = new TreeItem(*this,this->parentFolder->treeItem);
        treeItem->setRowIndex(parentFolder->subfolderList.count());
        if(check_exist && exists())
        {
            destExist = true;
        }
    }

    qDebug()<<"Folder init: sourceName: "<<sourceName;
    qDebug()<<"Folder init:sourcepath: "<<sourcePath;
    qDebug()<<"Folder init:destname: "<<destName;
}

void Folder::replace(bool replace)
{
    if(parentFolder->isReplace() || !destExist)
        return;

    if(replace && copyhandle!=COPY_REPLACE)
    {
        ignore(false);
        ignoreExisting(false);
        unrename();

        parentFolder->addSizeExistingFile(-fsizeExisting);
        parentFolder->addSizeReplace(fsizeExisting);
        copyhandle = COPY_REPLACE;
    }
    else if(!replace && copyhandle==COPY_REPLACE)
    {

        parentFolder->addSizeExistingFile(+fsizeExisting);
        parentFolder->addSizeReplace(-fsizeExisting);
        copyhandle = COPY_NOT_SET;
    }
}

void Folder::ignoreExisting(bool m_ignore)
{
    if(parentFolder->isIgnoreExisting() || !destExist)
        return;

    if(m_ignore && copyhandle!=COPY_IGNORE_EXISTING)
    {
        unrename();
        ignore(false);
        copyhandle = COPY_IGNORE_EXISTING;
        parentFolder->addSize(-fsizeExisting);
        parentFolder->addSizeExistingFile(-fsizeExisting);
    }
    else if(!m_ignore &&  copyhandle==COPY_IGNORE_EXISTING)
    {
        copyhandle = COPY_NOT_SET;
        parentFolder->addSize(fsizeExisting);
        parentFolder->addSizeExistingFile(fsizeExisting);
    }
}

bool Folder::isIgnoreExisting()
{
    if(copyhandle == COPY_IGNORE_EXISTING)
        return true;
    if(!parentFolder)
        return false;

    return parentFolder->isIgnoreExisting();
}

bool Folder::isReplace()
{
    if(copyhandle == COPY_REPLACE)
        return true;
    if(!parentFolder)
        return false;
    return parentFolder->isReplace();
}

bool Folder::isIgnore()
{
    if(copyhandle == COPY_IGNORE)
        return true;
    if(!parentFolder)
        return false;
    return parentFolder->isIgnore();
}

bool Folder::isRenamed()
{
    return this->sourceName != this->destName;
}

bool Folder::hasCopyStarted()
{
    return copyStarted;
}


void Folder::ignore(bool m_ignore)
{
    if( parentFolder->isIgnore() || (parentFolder->isIgnoreExisting() && destExist) )
        return;

    if(m_ignore && copyhandle!=COPY_IGNORE)
    {
        unrename();
        replace(false);
        ignoreExisting(false);
        copyhandle = COPY_IGNORE;
        if(destExist)
            parentFolder->addSizeExistingFile(-fsizeExisting);
        parentFolder->addSize(-fsize);
    }
    else if(!m_ignore && copyhandle==COPY_IGNORE)
    {
        copyhandle = COPY_NOT_SET;
        if(destExist)
            parentFolder->addSizeExistingFile(fsizeExisting);
        parentFolder->addSize(fsize);
    }else if(!m_ignore && copyhandle==COPY_IGNORE_EXISTING)
    {
        ignoreExisting(m_ignore);
    }
}

void Folder::unrename()
{
    if(copyhandle==COPY_RENAME)
    {
       destName = sourceName;
       copyhandle = COPY_NOT_SET;

       if(destExist)
           parentFolder->addSizeExistingFile(+fsizeExisting);
    }
}

bool Folder::rename(QString new_name)
{
    //get destination path
    if(copyhandle!=COPY_RENAME || !new_name.isEmpty())
    {
        QString path = parentFolder->getDestPath();
        ignoreExisting(false);
        ignore(false);
        replace(false);
        copyhandle = COPY_RENAME;
        if(destExist)
            parentFolder->addSizeExistingFile(-fsizeExisting);

        if( new_name.isEmpty() )//default rename
            new_name = QString(destName);

        int i=1;
        QString test = new_name;
        while(1)
        {
            QDir dir(path+test);
            if(test!=destName && !dir.exists())
                break;
            test = new_name;
            QString afix;
            afix.sprintf(" (%d)",i);
            test.append(afix);
            i++;
        }
        qDebug()<<"Folder: target from"<< sourcePath+sourceName  <<" renamed to: " << test;
        destName = test.toUtf8();
    }
    return true;
}


QString Folder::getSourcePath(bool fullPath)
{
    if(!fullPath){
        return sourcePath;
    }
    if(parentFolder != NULL )
    {
        QString path;
        if(!sourceName.isEmpty()){
            path = parentFolder->getSourcePath() + QString(sourceName) + QString("/");
        }else{
            path = parentFolder->getSourcePath() + QString("/");
        }

        return path;
    }

    if(!sourceName.isEmpty())
        return sourcePath+sourceName+"/";

    return QString::fromUtf8(sourcePath);
}

QString Folder::getDestName()
{
    return destName;
}

QString Folder::getSourceName()
{
    return sourceName;
}

QString Folder::getDestPath(bool fullPath)
{

    if( !fullPath ){
        return this->destPath;
    }

    if(parentFolder != NULL )
    {
        QString path;
        if(!destName.isEmpty()){
            path = parentFolder->getDestPath() + QString(destName) + QString("/");
        }else{
            path = parentFolder->getDestPath();
        }

        return path;
    }

    if( !destName.isEmpty() )
        return destPath+destName+"/";

    return destPath;
}

bool Folder::exists(bool reCheck)
{
    if(reCheck)
    {
        QString path = getDestPath();
        QDir dir(path);
        destExist = dir.exists();
    }
    return destExist;
}

double Folder::getReplaceSize()
{
    double size = 0;
    foreach(File *f,fileList)
    {

        //if started copy already started halfway
        if( f->hasCopyStarted() && !f->wasCopied() ){

            //remaining copy size
            size += f->getSize()-f->getCopiedSize();
        }

        if( !f->wasCopied() &&
                f->getCopyHandle()==File::COPY_REPLACE )
        {
            size += f->getTargetSize();
        }
    }

    foreach(Folder *f,subfolderList)
    {
        size += f->getReplaceSize();
    }

    return size;
}

double Folder::getExistingSize()
{
    return fsizeExisting;
}

double Folder::getSize()
{
    return fsize;
}

bool Folder::deleteSource()
{
    if(copyThreadInfo->testDestroy())
        return false;

    foreach(Folder *subfolder,subfolderList)
    {
        if( !subfolder->deleteSource() )
            return false;
    }

    foreach(File *f,fileList)
    {
        if( !f->deleteSource() )
            return false;
    }

    while(1)
    {
        QString fname = this->getSourcePath();

        QDir dir(fname);
        dir.refresh();
        QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries|QDir::Hidden|QDir::System,QDir::Name);

        if(!dir.exists() || list.count()>0)
        {
            return true;
        }

        if ( !dir.rmdir(fname) && dir.exists() )
        {
            int res;
            copyThreadInfo->sendMessage(QWidget::tr("Can't remove folder: ") + fname + QWidget::tr("\n retry?"),
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
    }

    return true;
}

bool Folder::deleteDestination()
{
    if(copyThreadInfo->testDestroy())
        return false;

    if( copyhandle == COPY_IGNORE || !copyStarted)
        return true;

    foreach(Folder *subfolder,subfolderList)
    {
        if( !subfolder->deleteDestination() )
            return false;
    }

    foreach(File *f,fileList)
    {
        if( !f->deleteDest() )
            return false;
    }

    while(1)
    {
        QString fname = this->getDestPath();

        QDir dir(fname);
        dir.refresh();
        QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries|QDir::Hidden|QDir::System,QDir::Name);

        if(!dir.exists() || list.count()>0)
        {
            return true;
        }

        if ( !dir.rmdir(fname) && dir.exists() )
        {
            int res;
            copyThreadInfo->sendMessage(QWidget::tr("Can't remove folder: ") + fname + QWidget::tr("\n retry?"),
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
    }
    return true;
}

void Folder::addSizeReplace(double size)
{
    fsizeReplace += size;
    if(parentFolder)
        parentFolder->addSizeReplace(size);
}

void Folder::load(QXmlStreamReader &doc)
{
    while(doc.readNextStartElement())
    {
        if( doc.name() == "FILE" )
        {
            QXmlStreamAttributes attr = doc.attributes();
            File *file = new File(copyThreadInfo,this,attr);
            this->fileList.append(file);
        }


        if( doc.name() == "FOLDER")
        {
            QXmlStreamAttributes attr = doc.attributes();
            Folder *f = new Folder(copyThreadInfo,this,attr);
            this->subfolderList.append(f);
            f->load(doc);
            continue;
        }
        doc.readNextStartElement();
    }
}

void Folder::addSizeExistingFile(double size,bool updated)
{
    Folder *folder = parentFolder;
    fsizeExisting += size;
    if(copyhandle == COPY_IGNORE_EXISTING && !updated)
    {
        updated = true;
        parentFolder->addSize(-size);
    }

    if(folder)
        folder->addSizeExistingFile( size, updated );
}

void Folder::addSize(double size)
{
    fsize += size;
    Folder *folder = parentFolder;
    if(folder)
        folder->addSize( size );

    if(!folder)
        copyThreadInfo->setTotalSize(fsize);
}


size_t Folder::filesCount()
{
    size_t count = 0;
    foreach(Folder *folder,subfolderList)
    {
        count += folder->filesCount();
    }
    return count + fileList.count();
}

size_t Folder::getTotalFileCount()
{
    size_t count = 0;

    foreach(Folder *folder,subfolderList)
    {
        count += folder->getTotalFileCount();
    }
    return count + fileList.count();
}

size_t Folder::getCopiedFilesCount()
{
    size_t count = 0;

    foreach(Folder *folder,subfolderList)
    {
        if(folder->copyhandle != COPY_IGNORE)
            count += folder->getCopiedFilesCount();
    }

    foreach(File *file,fileList)
    {
        if(file->getCopyHandle() != File::COPY_IGNORE)
        {
            if(file->hasCopyStarted())
                count++;
        }
    }
    return count;
}

size_t Folder::getToBeCopiedFilesCount()
{
    size_t count = 0;
    if(isIgnore())
        return 0;

    foreach(Folder *folder,subfolderList)
    {
        if(folder->copyhandle != COPY_IGNORE )
            count += folder->getToBeCopiedFilesCount();
    }

    foreach(File *file,fileList)
    {
        if(file->getCopyHandle() != File::COPY_IGNORE && !(file->exists() && isIgnoreExisting()) )
        {
            if(!file->wasCopied())
                count++;
        }
    }
    return count;
}



void Folder::traverse(QString path, FailHandle &handle)
{
    QDir dir(path);
    QFileInfoList list;
    while(1)
    {
        //include "./" and "../" to check if the folder can be accessed or is just empty
        list = dir.entryInfoList(QDir::AllEntries|QDir::Hidden|QDir::System,QDir::Name);
        if( list.isEmpty() )
        {
            int ans;

            if(handle==IgnoreAll){
                return;
            }

            copyThreadInfo->sendMessage(QWidget::tr("Can't open folder \"") + path + QWidget::tr("\"\n Retry?"),
                                    &ans,
                                    QMessageBox::Yes
                                        |QMessageBox::No
                                        |QMessageBox::Cancel
                                        |QMessageBox::NoAll);
            if(ans==QMessageBox::Yes)
            {
                continue;
            }

            if(ans==QMessageBox::No)
                return;

            if(ans==QMessageBox::Cancel)
            {
                copyThreadInfo->exit();
                return;
            }

            if(ans==QMessageBox::NoAll)
            {
                handle = IgnoreAll;
                return;
            }

        }else break;
    }

    list = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries|QDir::Hidden|QDir::System,QDir::Name);

    fileList.reserve(list.count());
    for( int i = 0;i < list.count(); i++)
    {
        if(copyThreadInfo->testDestroy())
            break;
        QFileInfo finfo = list[i];

        if(finfo.path().contains("/../") || finfo.path().contains("/./") ){
            continue;
        }

        if(finfo.isFile() || finfo.isSymLink())
        {
            addFile(finfo);
            continue;
        }

        if(finfo.isDir() && !finfo.isSymLink())
        {
            addFolder(finfo,handle);
            continue;
        }
    }
}

bool Folder::shouldRetry()
{
    return retry;
}

void Folder::setShouldRetry(bool retry)
{
    this->retry = retry;
}

void Folder::setCopyStarted(bool started)
{
    copyStarted = started;
}

Folder::CopyHandle Folder::getCopyHandle()
{
    return copyhandle;
}

void Folder::setCopyHandle(CopyHandle handle)
{
    copyhandle = handle;
}

void Folder::setFilesCopied()
{
    filesCopied = true;
}



QList<Folder *> Folder::getSubFolderList() const
{
    return subfolderList;
}

QList<File *> Folder::getFileList() const
{
    return fileList;
}

TreeItem* Folder::getTreeItem()
{
    return treeItem;
}

Folder *Folder::getParentFolder()
{
    return parentFolder;
}


void Folder::addFile(File* file, bool propagateSize)
{
    fileList.append(file);
    if(propagateSize)
    {
        addSize(file->getSize());
        if(file->exists())
            addSizeExistingFile(file->getSize());
    }else
    {
        this->fsize += file->getSize();
        if(file->exists())
            this->fsizeExisting += file->getSize();
    }
}

File* Folder::addFile(QFileInfo finfo, bool propagateSize)
{
    File *file = new File(copyThreadInfo,this,finfo,destExist);
    fileList.append(file);
    if(propagateSize)
    {
        addSize(file->getSize());
        if(file->exists())
            addSizeExistingFile(file->getSize());
    }else
    {
        this->fsize += file->getSize();
        if(file->exists())
            this->fsizeExisting += file->getSize();
    }
    return file;
}

void Folder::addFolder(Folder* folder, bool propagateSize)
{
    subfolderList.append(folder);

    FailHandle handle = Ask;
    folder->traverse(getSourcePath() + QString(folder->sourceName),handle );
    if(propagateSize)
    {
        addSize(folder->getSize());
        if(folder->exists())
            addSizeExistingFile(folder->fsizeExisting);
    }else
    {
        this->fsize += folder->getSize();
        this->fsizeExisting += folder->fsizeExisting;
    }
}

Folder* Folder::addFolder(QFileInfo finfo, FailHandle &handle)
{
    Folder *folder = new Folder(copyThreadInfo,this,finfo,destExist);
    subfolderList.append(folder);

    QString path = finfo.path();
    if( !path.endsWith("/") ){
        path.append("/");
    }

    folder->traverse(path+QString(folder->sourceName),handle );
    this->fsize += folder->fsize;
    this->fsizeExisting += folder->fsizeExisting;
    return folder;
}



//There are different kind of shortcuts, one is the "Folder shortcut"
//http://en.wikipedia.org/wiki/Symbolic_link#Folder_Shortcuts.5B11.5D
//to test if this is one we need to check the folder properties and
//the dekstop.ini file
QString Folder::getRealTargetPath(QString target)
{
    DWORD attr = GetFileAttributes(target.toStdWString().c_str());

    QString folderShortcutID = "{0AFACED1-E828-11D1-9187-B532F1E9575D}";

    qDebug()<<"Folder: target: "<<target;
    qDebug()<<"Folder: attributes:"<<attr;
    if( (attr&FILE_ATTRIBUTE_SYSTEM) || (attr&&FILE_ATTRIBUTE_READONLY) ){

        qDebug()<<"Folder: is system or read-only";
        if(!target.endsWith("/")){
            target+="/";
        }
        QString desktopIni = target+"desktop.ini";
        QSettings settings(desktopIni,QSettings::IniFormat);
        qDebug()<<"Folder: desktop.ini keys"<<settings.allKeys();
        if( settings.value(".ShellClassInfo/CLSID2") == folderShortcutID ){

            QFileInfo info(target+"target.lnk");

            target = info.symLinkTarget();

            QFileInfo symlink(target);
            if(symlink.isDir()){
                target += "/";
            }

        }
    }

    qDebug()<<"Folder: real target: "<<target;

    return target;
}
