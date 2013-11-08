
#include "ui_filelist.h"
#include "filelist.h"
#include "TaskThread/ReadableSize.h"
#include <QTreeWidget>
#include "TaskThread/mfolder.h"
#include <QDir>
#include <QMessageBox>
#include <QFileIconProvider>
#include <QMenu>
#include <QProcess>
#include <QProcessEnvironment>
#include <QDropEvent>
#include <QDateTime>
#include <QFontMetrics>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <string>
#include <iostream>
#include "ncsettings.h"
#include "TaskThread/taskthread.h"

#include <windows.h>
#include <QString>
filelist::filelist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::filelist)
{
    ui->setupUi(this);
    ui->progressBar->hide();

    m_model = new TreeModel();
    ui->treeWidget->setModel(m_model);


    treeWidget = ui->treeWidget;
    treeWidget->hideColumn(COL_PATH);

    treeWidget->resizeColumnToContents(1);
    treeWidget->resizeColumnToContents(2);
    treeWidget->resizeColumnToContents(3);
    treeWidget->resizeColumnToContents(4);
    QFontMetrics metrics(this->font());
    ui->treeWidget->setColumnWidth(COL_SIZE,metrics.width("100.100Kb"));
    treeWidget->resizeColumnToContents(6);
    treeWidget->resizeColumnToContents(7);
}

filelist::~filelist()
{
    top_items.clear();

    delete ui;
}

void filelist::SetTopList( QList<TreeItem*> &itemlist, TaskThread *thread )
{
    this->top_items = itemlist;
    mFolder *rootfolder = thread->Directories;

    if(thread->copy_type == LOCAL_NETWORK || thread->copy_type == FROM_INET)
    {
        m_model->isOnNetwork = true;
    }
    destination = QString::fromStdWString( rootfolder->dest_path + rootfolder->dest_name + tr("/").toStdWString());
}


void filelist::SetErrorList(mFolder *folder, QList<mFile*> &filelist )
{
    destination = QString::fromStdWString(folder->dest_path + folder->dest_name + tr("/").toStdWString());
    ui->cbtype->hide();

    STATE = STATE_LIST;
    ui->treeWidget->setColumnWidth(COL_IGNORE,200);
    QFileIconProvider ip;
    folderIcon = ip.icon(QFileInfo(QString::fromStdWString(folder->source_path + folder->source_name + tr("/").toStdWString())));

    ui->treeWidget->hideColumn(COL_REPLACE);
    ui->treeWidget->hideColumn(COL_RENAME);
    ui->treeWidget->hideColumn(COL_SIZE);
    ui->treeWidget->hideColumn(COL_DONE);
    ui->treeWidget->hideColumn(COL_EXISTS);
    ui->treeWidget->showColumn(COL_RETRY);

    for(int i=0; i < filelist.count();i++)
    {
        mFile *file = filelist[i];
        m_model->addTopLevelItem(file->treeitem);
    }
    ui->treeWidget->resizeColumnToContents(COL_NAME);
}

void filelist::ShowAll( )
{
    ui->treeWidget->hideColumn(COL_RETRY);

    STATE = STATE_ALL;

    m_model->setTopItems(this->top_items);

    ui->treeWidget->setColumnWidth(COL_NAME,150);
    QFontMetrics metrics(this->font());

    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->resizeColumnToContents(3);
    ui->treeWidget->resizeColumnToContents(5);
    ui->treeWidget->resizeColumnToContents(6);
    ui->treeWidget->resizeColumnToContents(7);
}

void filelist::ShowNewer()
{
    STATE = STATE_NEWER;
    QList<TreeItem*> recursiv_list;
    QList<TreeItem*> toplevelitemList;
    recursiv_list = this->top_items;
    if( recursiv_list.isEmpty() )
	return;

    int value = 0;
    if(top_items[0]->m_file)
	value = top_items[0]->m_file->parent_folder->filesCount();
    else
	value = top_items[0]->m_folder->parent_folder->filesCount();

    ui->progressBar->setMaximum(value);


    while(!recursiv_list.isEmpty())
    {
        TreeItem *item = recursiv_list[0];
        recursiv_list.removeFirst();
        if(item->childCount()>0)
        {
            //Folder
            if(item->m_folder->dest_exist)
            {
                for(int i=0;i<item->childCount();i++)
                {
                    TreeItem *child = item->child(i);
                    if(child->data(COL_EXISTS) == "Is Newer")
                    {
			 toplevelitemList.append(child);
                    }
                    if(child->childCount()>0)
                    {
                        if(child->m_folder->dest_exist)
                            recursiv_list.append(child);
                    }
                }
            }
        }else
        {
            //File
            if(item->data(COL_EXISTS) == "Is Newer")
            {
		 toplevelitemList.append(item);
            }
        }
	value--;
	ui->progressBar->setValue(ui->progressBar->maximum()-value);
	QApplication::processEvents();
    }
    m_model->setTopItems(toplevelitemList);
}

void filelist::ShowOlder()
{
    STATE = STATE_OLDER;
    QList<TreeItem*> recursiv_list;
    QList<TreeItem*> toplevelitemList;
    recursiv_list = this->top_items;
    if( recursiv_list.isEmpty() )
	return;

    int value = 0;
    if(top_items[0]->m_file)
	value = top_items[0]->m_file->parent_folder->filesCount();
    else
	value = top_items[0]->m_folder->parent_folder->filesCount();

    ui->progressBar->setMaximum(value);


    while(!recursiv_list.isEmpty())
    {
        TreeItem *item = recursiv_list[0];
        recursiv_list.removeFirst();
        if(item->childCount()>0)
        {
            //Folder
            if(item->m_folder->dest_exist)
            {
                for(int i=0;i<item->childCount();i++)
                {
                    TreeItem *child = item->child(i);
                    if(child->data(COL_EXISTS) == "Is Older")
                    {
			toplevelitemList.append(child);
                    }
                    if(child->childCount()>0)
                    {
                        if(child->m_folder->dest_exist)
                            recursiv_list.append(child);
                    }
                }
            }
        }else
        {
            //File
            if(item->data(COL_EXISTS) == "Is Older")
            {
		 toplevelitemList.append(item);
            }
        }
	value--;

	ui->progressBar->setValue(ui->progressBar->maximum()-value);
	QApplication::processEvents();
    }
    m_model->setTopItems(toplevelitemList);
}

bool filelist::ShowExist()
{
    //Be sure that the combobox shows "existing" when showing the "files exists"
    //error message
    this->blockSignals(true);
    ui->cbtype->setCurrentIndex(1);
    this->blockSignals(false);

    int value = this->top_items.count();
    ui->progressBar->setMaximum(value);
    STATE = STATE_CHECK;

    ui->treeWidget->hideColumn(COL_SIZE);
    ui->treeWidget->hideColumn(COL_DONE);
    ui->treeWidget->hideColumn(COL_RETRY);

    ui->treeWidget->showColumn(COL_RENAME);
    ui->treeWidget->showColumn(COL_REPLACE);
    ui->treeWidget->showColumn(COL_IGNORE);
    ui->treeWidget->showColumn(COL_EXISTS);
    ui->treeWidget->setColumnWidth(COL_NAME,150);

    QList<TreeItem*> toplevelitemList;

    for(int i=0; i < this->top_items.count();i++)
    {
        TreeItem *treeitem = this->top_items[i];

        if( treeitem->data(COL_EXISTS).isValid())
        {
	   toplevelitemList.append(treeitem);
        }
	value--;
	ui->progressBar->setValue(ui->progressBar->maximum()-value);
	QApplication::processEvents();
    }

    m_model->setTopItems(toplevelitemList);

    if( m_model->topLevelItems.count() > 0 )
        return true;
    else
        return false;
}

void filelist::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    QModelIndexList items = treeWidget->selectionModel()->selectedRows();
    if(items.count()==0)
        return;

    QMenu menu(this);
    menu.addAction("rename");
    menu.addAction("ignore existing");
    menu.addAction("ignore");

    menu.addAction("replace");

    if(STATE==STATE_LIST)
        menu.addAction("retry");

    if(items.count()==1)
    {
        menu.addSeparator();
        menu.addAction("Open destination");
        menu.addAction("Open source");
    }


    QAction *action = menu.exec(mapToGlobal( pos ));
    if(action == NULL)
        return;

    if( action->text() == "retry")
    {
        for(int i=0;i<items.count();i++)
        {
            TreeItem *item = (TreeItem*) items[i].internalPointer();
            item->m_folder->retry = true;
            ui->treeWidget->update(item->index);
        }
        return;
    }

    if( action->text() == "rename")
    {
        for(int i=0;i<items.count();i++)
        {
            TreeItem *item = (TreeItem*) items[i].internalPointer();
            if(!item->data(COL_RENAME).isValid())
            {
                item->rename(true);
                ui->treeWidget->update(items[i]);
            }
        }
        ui->treeWidget->resizeColumnToContents(COL_RENAME);
    }

    if( action->text() == "ignore existing")
    {
        for(int i=0;i<items.count();i++)
        {
            TreeItem *item = (TreeItem*) items[i].internalPointer();
            if(item->data(COL_IGNORE) != Qt::Checked)
            {
                item->ignoreExisting(true);
                ui->treeWidget->update(items[i]);
            }
        }
    }

    if( action->text() == "ignore")
    {
        for(int i=0;i<items.count();i++)
        {
            TreeItem *item = (TreeItem*) items[i].internalPointer();
            if(item->data(COL_IGNORE) != Qt::Checked)
            {
                item->ignore(true);
                ui->treeWidget->update(items[i]);
            }
        }
    }
    if( action->text() == "replace")
    {
        for(int i=0;i<items.count();i++)
        {
            TreeItem *item = (TreeItem*) items[i].internalPointer();
            if(item->data(COL_REPLACE) != Qt::Checked)
            {
                item->replace(true);
                ui->treeWidget->update(items[i]);
            }
        }
    }

    if(action->text() == "Open destination")
    {
        TreeItem *item = (TreeItem*) items[0].internalPointer();

        QString pathIn;

        if(item->m_folder)
        {
            mFolder* folder = item->m_folder;
            if(!folder->dest_exist)
                return;
            pathIn = folder->getDestPath();
        }else
        {
            mFile* file = item->m_file;
            if(!file->dest_exist && file->copyhandle!=mFile::COPY_RENAME)
                return;
            pathIn = QString::fromStdWString(file->parent_folder->getDestPath().toStdWString() + file->fDestName);
        }

        QStringList args;
        args << "/select," << QDir::toNativeSeparators(pathIn);

        QProcess::startDetached("explorer", args);
    }


    if(action->text() == "Open source")
    {
        TreeItem *item = (TreeItem*) items[0].internalPointer();

        QString pathIn;

        if(item->m_folder)
        {
            mFolder* folder = item->m_folder;
            pathIn = folder->getSourcePath();
            //check if folder exist
            // could be created manually
            QDir dir(pathIn);
            if(!dir.exists())
                return;
        }else
        {
            mFile* file = item->m_file;
            pathIn = QString::fromStdWString(file->fSourceName);
        }

        QStringList args;
        args << "/select," << QDir::toNativeSeparators(pathIn);

        QProcess::startDetached("explorer", args);
    }
}


void filelist::on_treeWidget_expanded(QModelIndex index)
{
    //Neat trick to know if we already handled this item, i had set it to "showIndicator"

    TreeItem *item = (TreeItem*) index.internalPointer();

    if(item->m_folder)
    {
        mFolder *folder = item->m_folder;
        for(int i=0; i < folder->subfolder_list.count();i++)
        {
            QApplication::processEvents();
            if(ncsettings::isExiting())
                break;
            mFolder *subfolder = folder->subfolder_list[i];

            if( STATE == STATE_CHECK)
            {
                //Only show files and folder that exists in destination
                // Hide the others
                if( subfolder->treeitem->data(COL_EXISTS) == Qt::Checked )
                {
                    ui->treeWidget->setRowHidden(i,folder->treeitem->index,true);
                }
            }else
            {
                //Show items that were hidden by other views
                if(ui->treeWidget->isRowHidden(i,folder->treeitem->index))
                    ui->treeWidget->setRowHidden(i,folder->treeitem->index,false);
            }
        }

        for(int i=0; i < folder->file_list.count();i++)
        {
            QApplication::processEvents();
            if(ncsettings::isExiting())
                break;
            mFile *file = (mFile*) folder->file_list[i];
            if( STATE == STATE_CHECK)
            {
                //Only show files and folder that exists in destination
                // Hide the others
                if( file->treeitem->data(COL_EXISTS) == Qt::Checked )
                {
                    ui->treeWidget->setRowHidden(i,file->treeitem->index,true);
                }
            }else
            {
                //Show items that were hidden by other views
                if(ui->treeWidget->isRowHidden(i,file->treeitem->index))
                    ui->treeWidget->setRowHidden(i,file->treeitem->index,false);
            }
        }
    }
}


void filelist::on_cbtype_activated(int index)
{
    //remove all items first
    ui->cbtype->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    m_model->removeAllTopItems();

    switch(index)
    {
    case 0:
        ShowAll();
        break;
    case 1:
        ShowExist();
        break;
    case 2:
        ShowNewer();
        break;
    case 3:
        ShowOlder();
        break;
    }
    ui->cbtype->setEnabled(true);
    ui->progressBar->hide();
}

void filelist::on_ckfullpath_stateChanged(int state)
{
    if(state == Qt::Checked)
    {
        treeWidget->showColumn(COL_PATH);
        treeWidget->setColumnWidth(COL_PATH,100);
    }else
    {
        treeWidget->hideColumn(COL_PATH);
    }
}

void filelist::on_searchInput_returnPressed()
{
    if(ui->searchInput->text().isEmpty())
        return;
    QModelIndex index =  m_model->index(0,0);

    if(searchitemlist.isEmpty())
    {
        searchtext.clear();
    }
    if(searchtext != ui->searchInput->text())
    {
        searchtext = ui->searchInput->text();
        searchitemlist = m_model->match(index,Qt::DisplayRole,searchtext,-1,Qt::MatchContains|Qt::MatchRecursive);
        if(searchitemlist.isEmpty())
        {
            ui->LbsearchResult->setText("nothing found!");
            return;
        }
        searchresult = searchitemlist.count();
    }
    index = searchitemlist.takeFirst();
    ui->LbsearchResult->setText(QString("found: %1,current: %2").arg(searchresult).arg(searchresult-searchitemlist.count()));
    treeWidget->scrollTo(index);
    treeWidget->setCurrentIndex(index);
}
