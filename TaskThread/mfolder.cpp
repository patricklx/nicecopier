#include "mFolder.h"
#include <QDir>
#include <QMessageBox>
#include <QFileInfoList>
#include <QFileInfo>
#include "taskthread.h"
#include <windows.h>
#include "ReadableSize.h"
#include <QFileIconProvider>
#include <qdatetime.h>

mFolder::mFolder(TaskThread *thread,mFolder *parent, QFileInfo folderinfo,bool check_exist)
{
    files_copied = false;
    fsize = 0;
    parent_folder = parent;
    started_copy = false;
    CopyThreadInfo = thread;
    dest_exist = false;
    copyhandle = COPY_NOT_SET;
    fsize_existing  = 0;
    fsize_replace = 0;


    source_name = folderinfo.fileName().toStdWString();
    dest_name = folderinfo.fileName().toStdWString();
    source_path = folderinfo.path().toStdWString();
    treeitem = NULL;
    if(this->parent_folder)
    {
        treeitem = new TreeItem(*this,this->parent_folder->treeitem);
	if(check_exist && exists())
        {
            dest_exist = true;
        }
    }
}

void mFolder::replace(bool replace)
{
    if(parent_folder->isReplace() || !dest_exist)
        return;

    if(replace && copyhandle!=mFolder::COPY_REPLACE)
    {
        ignore(false);
        ignoreExisting(false);
        unrename();

        parent_folder->addSizeExistingFile(-fsize_existing);
        parent_folder->addSizeReplace(fsize_existing);
        copyhandle = mFolder::COPY_REPLACE;
    }
    else if(!replace && copyhandle==mFolder::COPY_REPLACE)
    {

        parent_folder->addSizeExistingFile(+fsize_existing);
        parent_folder->addSizeReplace(-fsize_existing);
        copyhandle = mFolder::COPY_NOT_SET;
    }
}

void mFolder::ignoreExisting(bool m_ignore)
{
    if(parent_folder->isIgnoreExisting() || !dest_exist)
        return;

    if(m_ignore && copyhandle!=COPY_IGNORE_EXISTING)
    {
        unrename();
        ignore(false);
        copyhandle = COPY_IGNORE_EXISTING;
        parent_folder->addSize(-fsize_existing);
        parent_folder->addSizeExistingFile(-fsize_existing);
    }
    else if(!m_ignore &&  copyhandle==COPY_IGNORE_EXISTING)
    {
        copyhandle = COPY_NOT_SET;
        parent_folder->addSize(fsize_existing);
        parent_folder->addSizeExistingFile(fsize_existing);
    }
}

bool mFolder::isIgnoreExisting()
{
    if(copyhandle == COPY_IGNORE_EXISTING)
        return true;
    if(!parent_folder)
        return false;

    return parent_folder->isIgnoreExisting();
}

bool mFolder::isReplace()
{
    if(copyhandle == COPY_REPLACE)
        return true;
    if(!parent_folder)
        return false;
    return parent_folder->isReplace();
}

bool mFolder::isIgnore()
{
    if(copyhandle == COPY_IGNORE)
        return true;
    if(!parent_folder)
        return false;
    return parent_folder->isIgnore();
}


void mFolder::ignore(bool m_ignore)
{
    if( parent_folder->isIgnore() || (parent_folder->isIgnoreExisting() && dest_exist) )
        return;

    if(m_ignore && copyhandle!=COPY_IGNORE)
    {
        unrename();
        replace(false);
        ignoreExisting(false);
        copyhandle = COPY_IGNORE;
        if(dest_exist)
            parent_folder->addSizeExistingFile(-fsize_existing);
        parent_folder->addSize(-fsize);
    }
    else if(!m_ignore && copyhandle==COPY_IGNORE)
    {
        copyhandle = COPY_NOT_SET;
        if(dest_exist)
            parent_folder->addSizeExistingFile(fsize_existing);
        parent_folder->addSize(fsize);
    }else if(!m_ignore && copyhandle==COPY_IGNORE_EXISTING)
    {
        ignoreExisting(m_ignore);
    }
}

void mFolder::unrename()
{
    if(copyhandle==COPY_RENAME)
    {
       dest_name = source_name;
       copyhandle = COPY_NOT_SET;

       if(dest_exist)
           parent_folder->addSizeExistingFile(+fsize_existing);
    }
}

bool mFolder::rename(QString new_name)
{
    //get destination path
    QString path = parent_folder->getDestPath();
    ignoreExisting(false);
    ignore(false);
    replace(false);
    copyhandle = COPY_RENAME;
    if(dest_exist)
        parent_folder->addSizeExistingFile(-fsize_existing);

    if( new_name.isEmpty() )//default rename
        new_name = QString::fromStdWString(dest_name);

    int i=1;
    QString test = new_name;
    while(1)
    {
        test = new_name;
        QString afix;
        afix.sprintf(" (%d)",i);
        test.append(afix);
        QDir dir(path+test);
        if(!dir.exists())
            break;
        i++;
    }
    dest_name = test.toStdWString();

    return true;
}


QString mFolder::getSourcePath()
{
    if(parent_folder != NULL)
    {
        QString path = parent_folder->getSourcePath() + QString::fromStdWString(source_name) + QString("/");
        return path;
    }
    return QString::fromStdWString(source_path + source_name) + QString("/");
}

QString mFolder::getDestPath()
{
    if(parent_folder != NULL)
    {
        QString path = parent_folder->getDestPath() + QString::fromStdWString(source_name) + QString("/");
        return path;
    }

    return QString::fromStdWString(dest_path + dest_name) + QString("/");
}

bool mFolder::exists()
{
    QString path = parent_folder->getDestPath();
    QDir dir(path+QString::fromStdWString(dest_name));
    return dir.exists();
}

double mFolder::getReplaceSize()
{
    return fsize_replace;
}

bool mFolder::DeleteSource()
{
    int i=0;
    for( i = 0; i < subfolder_list.count();i++)
    {
        mFolder *folder = (mFolder*) subfolder_list[i];
        if( !folder->DeleteSource() )
            return false;
    }

    for( i = 0; i < file_list.count();i++)
    {
        mFile *f = (mFile*) file_list[i];
        if( !f->DeleteSource() )
            return false;
    }

    while(1)
    {
        QString fname = QString::fromStdWString(source_path + source_name);
        QDir dir(fname);
        if(!dir.exists())
            return true;

        if ( !dir.rmdir(fname) )
        {
            int res;
            CopyThreadInfo->message(QString("Can't remove folder: ") + fname + QString("\n retry?"),&res,QMessageBox::No|QMessageBox::Cancel
                                                                                               |QMessageBox::Yes);
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

bool mFolder::DeleteDest()
{
    if( copyhandle == COPY_IGNORE || !started_copy)
        return true;
    int i = 0;
    for(i=0; i < subfolder_list.count();i++)
    {
        mFolder *folder = (mFolder*) subfolder_list[i];
        if( !folder->DeleteDest() )
            return false;
    }

    for(i=0; i < file_list.count();i++)
    {
        mFile *f = (mFile*) file_list[i];
        if( !f->DeleteDest() )
            return false;
    }

    while(1)
    {
        QString fname = QString::fromStdWString(dest_path + dest_name);
        if( dest_path.length()==0 )
        {
            return true;
        }
        QDir dir(fname);
        if(!dir.exists())
            return true;
        if ( !dir.rmdir(fname) )
        {
            int res;
            CopyThreadInfo->message(QString("Can't remove folder: ") + fname + QString("\n retry?"),&res,QMessageBox::No|QMessageBox::Cancel|QMessageBox::Yes);
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

void mFolder::addSizeReplace(double size)
{
    fsize_replace += size;
    if(parent_folder)
        parent_folder->addSizeReplace(size);
}

void mFolder::addSizeExistingFile(double size,bool updated)
{
    mFolder *folder = parent_folder;
    fsize_existing += size;
    if(copyhandle == COPY_IGNORE_EXISTING && !updated)
    {
        updated = true;
        parent_folder->addSize(-size);
    }

    if(folder)
        folder->addSizeExistingFile( size, updated );
}

void mFolder::addSize(double size)
{
    fsize += size;
    mFolder *folder = parent_folder;
    if(folder)
        folder->addSize( size );

    if(!folder)
        CopyThreadInfo->TotalSize = fsize;
}


size_t mFolder::filesCount()
{
    size_t count = 0;
    for(int i = 0; i < subfolder_list.count();i++)
    {
	mFolder *folder = (mFolder*) subfolder_list[i];
	count += folder->filesCount();
    }
    return count + file_list.count();
}

size_t mFolder::getTotalFileCount()
{
    size_t count = 0;
    for(int i = 0; i < subfolder_list.count();i++)
    {
        mFolder *folder = (mFolder*) subfolder_list[i];
        if(folder->copyhandle != COPY_IGNORE)
            count += folder->getTotalFileCount();
    }
    return count + file_list.count();
}

size_t mFolder::getCopiedFilesCount()
{
    size_t count = 0;
    for(int i = 0; i < subfolder_list.count();i++)
    {
        mFolder *folder = (mFolder*) subfolder_list[i];
        if(folder->copyhandle != COPY_IGNORE)
            count += folder->getCopiedFilesCount();
    }

    for(int i = 0; i < file_list.count();i++)
    {
        mFile *file = (mFile*) file_list[i];
        if(file->copyhandle != mFile::COPY_IGNORE)
        {
            if(file->copied)
                count++;
        }
    }
    return count;
}

size_t mFolder::getToBeCopiedFilesCount()
{
    size_t count = 0;
    for(int i = 0; i < subfolder_list.count();i++)
    {
        mFolder *folder = (mFolder*) subfolder_list[i];
        if(!folder->copyhandle != COPY_IGNORE)
            count += folder->getToBeCopiedFilesCount();
    }

    for(int i = 0; i < file_list.count();i++)
    {
        mFile *file = (mFile*) file_list[i];
        if(!file->copyhandle != COPY_IGNORE)
        {
            if(!file->copied)
                count++;
        }
    }
    return count;
}


void mFolder::Traverse(TaskThread *info,QString path)
{
    QDir dir(path);

    QFileInfoList list;
    while(1)
    {
        list = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries|QDir::Hidden,QDir::Name);
        if(list.isEmpty() && !dir.isReadable())
        {
            int ans;
            CopyThreadInfo->message(QString("Can't open folder \"") + QString("\"") + path + QString("\n Retry?"),
                                    &ans,
                                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(ans==QMessageBox::Yes)
            {
                continue;
            }

            if(ans==QMessageBox::No)
                return;

            if(ans==QMessageBox::Cancel)
            {
                CopyThreadInfo->Exit();
                return;
            }
        }else break;
    }

    for( int i = 0;i < list.count(); i++)
    {
        if(CopyThreadInfo->TestDestroy())
            break;
        QFileInfo finfo = list[i];
        if(finfo.isFile() || finfo.isSymLink())
        {
	    mFile *file = new mFile(info,this,finfo,dest_exist);
            file->parent_folder = this;
            this->fsize += file->fsize;
            if(file->dest_exist)
                this->fsize_existing += file->fsize;
            file_list.append(file);
            continue;
        }
        if(finfo.isDir())
        {
	    mFolder *folder = new mFolder(info,this,finfo,dest_exist);

            folder->parent_folder = this;
            subfolder_list.append(folder);

            folder->Traverse(info, path + QString::fromStdWString(folder->source_name) + QString("/") );
            this->fsize += folder->fsize;
            this->fsize_existing += folder->fsize_existing;
            continue;
        }
    }
}

mFolder::~mFolder()
{
    while( !file_list.isEmpty() )
    {
        mFile *file = (mFile*)file_list[0];
        file_list.removeFirst();
        delete file;
    }

    while( !subfolder_list.isEmpty() )
    {
        mFolder *folder = (mFolder*) subfolder_list[0];
        subfolder_list.removeFirst();
        delete folder;
    }
    if(treeitem)
        delete treeitem;
}



