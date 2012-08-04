#include "treeitem.h"
#include "TaskThread/File.h"
#include "TaskThread/Folder.h"
#include <QFileInfo>
#include "TaskThread/ReadableSize.h"
#include <QMessageBox>
#include <QApplication>

#include <QFileIconProvider>

#include "ncsettings.h"
#include <QDebug>

TreeItem::TreeItem()
{
    file = NULL;
    folder = NULL;
    topLevelItem = false;
    parentItem = NULL;
}

TreeItem::TreeItem(Folder &folder, TreeItem *parent)
{
    this->folder = &folder;
    parentItem = parent;
    file = NULL;
    topLevelItem = false;
}

TreeItem::TreeItem(File &file, TreeItem *parent)
{
    parentItem = parent;
    this->file = &file;
    folder = NULL;
    topLevelItem = false;
}

TreeItem::~TreeItem()
{
}

QModelIndex TreeItem::getModelIndex() const
{
    return modelIndex;
}

void TreeItem::setModelIndex(QModelIndex index)
{
    modelIndex = index;
}

TreeItem *TreeItem::child(int row)
{
    if(folder)
    {
        int folder_count = folder->getSubFolderList().count();
        if(row < folder_count)
        {
            QList<Folder*> list = folder->getSubFolderList();
            return list.at(row)->getTreeItem();
        }
        else
        {
            QList<File*> list = folder->getFileList();
            return list.at(row-folder_count)->getTreeItem();
        }
    }
    return NULL;
}

int TreeItem::childCount() const
{
    if(folder)
        return folder->getFileList().count()+folder->getSubFolderList().count();
    return 0;
}

int TreeItem::columnCount() const
{
    return 9;
}

void TreeItem::setTopLevelItem(bool isTopLvel)
{
    topLevelItem = isTopLvel;
}

bool TreeItem::isTopLevelItem()
{
    return topLevelItem;
}


QVariant TreeItem::data(int column,int role)
{
    switch(column)
    {
        case COL_NAME:
        {
            if(role == Qt::DisplayRole)
            {
                if(file)
                    return file->getSourceName().afterLast("/");
                if(folder)
                    return folder->getSourceName();
            }
        }
        break;
        case COL_PATH:
        {
            if(role == Qt::DisplayRole)
            {
                /*if(file)
                    return file->getSourceName().beforeLast("/");
                if(folder)
                    return folder->getParentFolder()->getSourcePath();*/
            }
        }
        break;
        case COL_IGNORE:
        {
            if(role == Qt::CheckStateRole)
            {
                if(file)
                {
                    if( file->getParentFolder()->isIgnore() && file->getCopyHandle()==File::COPY_NOT_SET )
                        return Qt::Checked;

                    if( file->getParentFolder()->isIgnoreExisting() && file->exists() &&  file->getCopyHandle()==File::COPY_NOT_SET )
                        return Qt::Checked;

                    return (file->getCopyHandle() == File::COPY_IGNORE)*Qt::Checked;
                }

                if(folder)
                {
                    if( folder->getParentFolder()->isIgnore() && folder->getCopyHandle()==Folder::COPY_NOT_SET )
                        return Qt::Checked;

                    if( folder->getParentFolder()->isIgnoreExisting() && folder->exists() &&  folder->getCopyHandle()==Folder::COPY_NOT_SET )
                        return Qt::Checked;

                    return ((folder->getCopyHandle() == Folder::COPY_IGNORE) ||
                            (folder->getCopyHandle() == Folder::COPY_IGNORE_EXISTING ))*Qt::Checked;
                }
                return Qt::Unchecked;
            }
            if(role == Qt::DisplayRole)
            {
                if(file)
                {
                    if(!file->getSourceError().isEmpty() || !file->getDestinationError().isEmpty())
                    {
                        if(!file->getSourceError().isEmpty())
                            return QString("Source: " + file->getSourceError());
                        else
                            return QString("Destination: "+ file->getDestinationError());
                    }
                }
                if(folder)
                {
                    if( folder->getCopyHandle()==Folder::COPY_IGNORE_EXISTING )
                        return "ignore existing";
                    if( folder->getParentFolder()->isIgnoreExisting() && folder->exists() && folder->getCopyHandle()==Folder::COPY_NOT_SET )
                        return "ignore existing";
                }
            }
        }
        break;
        case COL_REPLACE:
        {
            if(role == Qt::CheckStateRole)
            {
                if(file && file->exists())
                {
                    if( file->getParentFolder()->isReplace() && file->getCopyHandle()==File::COPY_NOT_SET)
                        return Qt::Checked;
                    return (file->getCopyHandle() == File::COPY_REPLACE)*Qt::Checked;
                }
                if(folder && folder->exists())
                {
                    if( folder->getParentFolder()->isReplace() && folder->getCopyHandle()==Folder::COPY_NOT_SET)
                        return Qt::Checked;

                    return (folder->getCopyHandle() == Folder::COPY_REPLACE)*Qt::Checked;
                }
            }
        }
        break;
        case COL_RENAME:
        {
            if(role == Qt::DisplayRole)
            {
                if(file)
                {
                    if( file->isRenamed() )
                        return file->getDestinationName().afterLast("/");
                }
                if(folder)
                {
                    if(folder->isRenamed())
                        return folder->getDestName();
                }
            }
            if(role == Qt::CheckStateRole)
            {
                if(file)
                {
                    return (file->isRenamed())*Qt::Checked;
                }
                if(folder)
                {
                    return (folder->isRenamed())*Qt::Checked;
                }
            }
        }
        break;
        case COL_RETRY:
        {
            if(role == Qt::CheckStateRole)
            {
                if(file)
                    return file->shouldRetry()*Qt::Checked;
            }
        }
        break;
        case COL_SIZE:
        {
            if(role == Qt::DisplayRole)
            {
                if(file && file->getSize()>0)
                    return Util::toReadableSize(file->getSize());
                if(folder && folder->getSize()>0)
                    return Util::toReadableSize(folder->getSize());
            }
        }
        break;
        case COL_EXISTS:
        {
            if(role == Qt::DisplayRole)
            {
                if(file)
                {
                    if(file->exists() && file->getCopyHandle()!=File::COPY_RENAME)
                    {
                        if(file->compareAge() == File::OLDER)
                            return "Older";
                        if(file->compareAge() == File::NEWER)
                            return "Newer";
                        if(file->compareAge() == File::EQUAL)
                        {
                            if(file->compareSize()==File::SAME)
                                return "Same Date and Size";
                            return "Same Date";
                        }
                    }
                }
                if(folder)
                {
                    if(folder->exists())
                        return "Exists";
                }
            }
        }
        break;
        /*case COL_DONE:
            if(role == Qt::DisplayRole)
            {
                if(file)
                {
                    if(file->getSize() == file->getCopiedSize())
                        return ("DONE");
                    return Util::toReadableSize(file->getCopiedSize());
                }
            }
        break;*/

        case COL_CHECKSUM_STAT:

            if(role == Qt::DisplayRole)
            {
                if(file){
                    switch(file->getChecksumStatus())
                    {
                        case File::PASSED:
                            return "PASSED";
                        break;
                        case File::FAILED:
                            return "FAILED";
                        break;
                        case File::NOT_TESTED:
                            return "NOT_TESTED";
                        break;
                    }
                }
            }
            break;

        case COL_DEBUG:
            if(role == Qt::DisplayRole)
            {
                int replace = folder?folder->getUsedDiskSpace():file->getSize();
                int exist = folder?folder->getExistingSize():0;
                int size = folder?folder->getSize():file->getSize();

                return QString("size:%1 ,replace:%2 ,existing: %3").arg(size).arg(replace).arg(exist);
            }
        break;
    }
    return QVariant();
}

TreeItem *TreeItem::parent()
{
    if(file && file->getParentFolder())
        return file->getParentFolder()->getTreeItem();

    if(folder && folder->getParentFolder())
        return folder->getParentFolder()->getTreeItem();

    return NULL;
}


void TreeItem::setRowIndex(int index)
{
    rowIndex = index;
}

int TreeItem::getRow()
{
    TreeItem *_parent = parent();
    if (_parent)
    {
        return rowIndex;
    }
    return 0;
}

void TreeItem::rename(bool rename,QString name)
{
    if(this->folder)
    {
        if(this->folder->hasCopyStarted())
            return;

        Folder *folder = this->folder;
        if(rename)
            folder->rename(name);
        else
            folder->unrename();
    }else
    {
        File *file = this->file;
        if(file->hasCopyStarted())
            return;
        if(rename)
        {
            bool ok = file->rename(name);
            if(!ok)
            {
                QString msg = QString("Error renaming file to \"") + name + QString("\"!\n The file already exists\n");
                QMessageBox::information(QApplication::activeWindow(),"NiceCopier rename",msg);

            }
        }else
            file->unrename();
    }
}

void TreeItem::ignore(bool ignore)
{
    if(this->folder)
    {
        if(this->folder->hasCopyStarted())
            return;
        Folder *folder = this->folder;
        folder->ignore(ignore);
    }else
    {
        File *file = this->file;
        file->ignore(ignore);
    }
}

void TreeItem::ignoreExisting(bool ignore)
{
    if(this->folder)
    {
        if(this->folder->hasCopyStarted())
            return;
        Folder *folder = this->folder;
        folder->ignoreExisting(ignore);
    }else
    {
        File *file = this->file;
        if(file->hasCopyStarted())
            return;
        file->ignoreExisting(ignore);
    }
}

void TreeItem::replace(bool replace)
{
    //QApplication::activeWindow()->setCursor(Qt::WaitCursor);
    if(this->file)
    {
        File *file = this->file;
        if(file->hasCopyStarted())
            return;
        file->replace(replace);
    }else
    {
        if(this->folder->hasCopyStarted())
            return;
        Folder *folder = this->folder;
        folder->replace(replace);
    }
    //QApplication::activeWindow()->setCursor(Qt::ArrowCursor);
}

File *TreeItem::getFile()
{
    return file;
}

Folder *TreeItem::getFolder()
{
    return folder;
}


TreeModel::TreeModel()
    : QAbstractItemModel()
{
    headers << "File" << "path" << "ignore" << "replace" << "rename" << "Size" << "Target:" << "CheckSum Test" << "retry";
    onNetwork = false;
}

TreeModel::~TreeModel()
{
    removeAllTopItems();
}

QList<TreeItem *> TreeModel::getTopLevelItems()
{
    return topLevelItems;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return headers.count();
}


void TreeModel::setTopItems(QList<TreeItem*> &top_items)
{
    removeAllTopItems();

    for(int i=0;i<top_items.count();i++)
    {
        addTopLevelItem(top_items[i]);
    }
}

void TreeModel::removeAllTopItems()
{
    if(topLevelItems.count()>0)
    {
        beginRemoveRows(QModelIndex(),0,topLevelItems.count()-1);
        while(!topLevelItems.isEmpty())
        {
            TreeItem *item = topLevelItems[0];
            item->setTopLevelItem(false);
            topLevelItems.removeFirst();
        }
        endRemoveRows();
    }
}

void TreeModel::removeTopLevelItem(TreeItem *item)
{
    int row = item->getRow();
    beginRemoveRows(QModelIndex(),row,row);

    topLevelItems.removeOne(item);
    item->setTopLevelItem(false);

    endRemoveRows();
}

void TreeModel::addTopLevelItem(TreeItem *top_item)
{
    top_item->setTopLevelItem(true);
    int row = topLevelItems.count();
    beginInsertRows(QModelIndex(),row,row);
    topLevelItems.append(top_item);
    endInsertRows();
}


void TreeModel::updateItem(TreeItem *item)
{
    QModelIndex startindex = createIndex(item->getModelIndex().row(),0,item);
    if(!startindex.isValid())
        startindex = createIndex(item->getRow(),0,item);

    QModelIndex endindex = createIndex(startindex.row(),8,item);
    dataChanged(startindex,endindex);
}

int sortColumn = COL_NAME;
Qt::SortOrder sortOrder = Qt::AscendingOrder;
bool sortFunction(TreeItem *i1,TreeItem *i2)
{
    switch(sortColumn)
    {
        case COL_NAME:
        case COL_PATH:
        case COL_EXISTS:
        case COL_RETRY:
        case COL_CHECKSUM_STAT:
            return i1->data(sortColumn).toString() < i2->data(sortColumn).toString();
        break;
        case COL_SIZE:
        {
            File *f1 = (File*)i1->getFile();
            File *f2 = (File*)i2->getFile();

            if( f1 && f2 ){

                return (f1->getSize() < f2->getSize());
            }
        }
        break;
    }
    return false;
}

void TreeModel::sort(int column, Qt::SortOrder order)
{
    sortColumn = column;
    sortOrder = order;
    //qSort(topLevelItems.begin(),topLevelItems.end(),sortFunction);
}


bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TreeItem *item = (TreeItem*) index.internalPointer();

    switch(index.column())
    {
        case COL_RENAME:
        {
            if(role == Qt::CheckStateRole)
            {
                item->rename(value.toBool(),"");
            }
            if(role == Qt::EditRole)
            {
                item->rename(true,value.toString());
            }
        }
        break;
        case COL_REPLACE:
        {
            if(role == Qt::CheckStateRole)
            {
                item->replace(value.toBool());

            }
        }
        break;
        case COL_IGNORE:
        {
            if(role == Qt::CheckStateRole)
            {
                item->ignore(value.toBool());
                if(item->getFile())
                    item->getFile()->setRetry(false);
            }
        }
        break;
        case COL_RETRY:
        {
            if(role == Qt::CheckStateRole)
            {
                if(item->getFile())
                    item->getFile()->setRetry(value.toBool());

                item->ignore(false);
            }
        }
    }
    updateItem(item);
    return true;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if(role == Qt::DecorationRole)
    {
        if(index.column()==COL_NAME)
        {
            if(item->getFile())
            {
                if(onNetwork || NcSettings::getValue<bool>(NcSettings::DEFAULT_ICONS))
                {
                    return iconProvider.icon(QFileIconProvider::File);
                }
                QIcon icon = iconProvider.icon(QFileInfo(item->getFile()->getSourceName()));
                if(icon.isNull())
                    return iconProvider.icon(QFileIconProvider::File);
                return icon;
            }
            if(item->getFolder())
            {
                QString path = item->getFolder()->getSourcePath();
                if(onNetwork)
                {
                    return iconProvider.icon(QFileIconProvider::Folder);
                }
                QIcon icon = iconProvider.icon(QFileInfo(path));
                if(icon.isNull())
                    return iconProvider.icon(QFileIconProvider::Folder);
                return icon;
            }
        }
    }

    if(role == Qt::ToolTipRole)
    {
        return item->data(index.column(),Qt::DisplayRole);
    }

    if(role == Qt::EditRole && index.column()==COL_RENAME)
    {
        return item->data(index.column(),Qt::DisplayRole);
    }

    if (role != Qt::DisplayRole && role != Qt::CheckStateRole)
        return QVariant();

    return item->data(index.column(),role);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    int flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    TreeItem *item = (TreeItem*) index.internalPointer();

    File *f = item->getFile();
    if( f && (f->hasCopyStarted()
        && f->getDestinationError().isEmpty()
        && f->getSourceError().isEmpty())){

        return Qt::ItemIsSelectable;
    }

    if( item->getFolder() && item->getFolder()->hasCopyStarted() ){

        return Qt::ItemIsSelectable;
    }

    if(index.column() == COL_RENAME && item->data(COL_RENAME,Qt::CheckStateRole)==Qt::Checked)
    {
        flags = flags | Qt::ItemIsEditable;
    }
    if(index.column()==COL_REPLACE ||
       index.column()==COL_IGNORE ||
       index.column()==COL_RENAME ||
       index.column()==COL_RETRY)
    {
        bool parentIgnored = item->getFile()?item->getFile()->getParentFolder()->isIgnore():item->getFolder()->getParentFolder()->isIgnore();
        bool parentReplaced = item->getFile()?item->getFile()->getParentFolder()->isReplace():item->getFolder()->getParentFolder()->isReplace();
        bool parentIgnoredExist = item->getFile()?item->getFile()->getParentFolder()->isIgnoreExisting():item->getFolder()->getParentFolder()->isIgnoreExisting();
        bool exists = item->getFile()?item->getFile()->exists():item->getFolder()->exists();
        flags = flags | Qt::ItemIsUserCheckable;

        if(index.column()==COL_REPLACE && (parentReplaced || !exists ) )
            flags = flags&~Qt::ItemIsEnabled;

        if(index.column()==COL_IGNORE && parentIgnoredExist && exists)
            flags = flags&~Qt::ItemIsEnabled;

        if(parentIgnored)
            flags = flags&~Qt::ItemIsEnabled;
    }
    return (Qt::ItemFlags)flags;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return headers[section];

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = NULL;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem;
    if(parentItem==NULL)
        childItem = topLevelItems[row];
    else
        childItem = parentItem->child(row);
    if (childItem)
    {
        childItem->setModelIndex( createIndex(row, column, childItem) );
        return childItem->getModelIndex();
    }
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == NULL || childItem->isTopLevelItem())
        return QModelIndex();

    return createIndex(parentItem->getRow(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
    {
        return topLevelItems.count();
    }
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
