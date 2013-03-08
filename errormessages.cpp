#include "errormessages.h"
#include "ui_errormessages.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include "TaskThread/ReadableSize.h"
#include <QApplication>
#include <QDesktopWidget>
#include <ncsettings.h>
#include <QCloseEvent>


DlgErrorMessages::DlgErrorMessages(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorMessages)
{
    ui->setupUi(this);
    ui->BtRetry->hide();

    setWindowTitle(tr("NiceCopier message"));


    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowMinMaxButtonsHint|Qt::Dialog);

    adjustSize();

    this->move(NcSettings::screenCenter()-rect().bottomRight()/2);

}

DlgErrorMessages::~DlgErrorMessages()
{
    delete ui;
}


bool DlgErrorMessages::setErrors(TaskThread *thread, int type)
{
    m_thread = thread;
    m_error_type = type;
    //Set top items list
    QList<TreeItem*> items;

    foreach(Folder *f, thread->getCopyDirectory()->getSubFolderList())
    {
        items.append(f->getTreeItem());
    }

    foreach(File *f, thread->getCopyDirectory()->getFileList())
    {
        items.append(f->getTreeItem());
    }

    ui->treeWidget->setTopList(items,thread);

    if(type==ER_FILE_DEST_EXIST)
    {
        bool ok = ui->treeWidget->showExist();
        ui->lbDestination->setText(ui->treeWidget->getDestination());
        ui->lbDestination->setToolTip(ui->lbDestination->text());
        ui->lbSource->setText(thread->getSourcePath());
        ui->lbSource->setToolTip(ui->lbSource->text());

        qDebug()<<"DlgErrorMessages: "<<"file exists";
        qDebug()<<"DlgErrorMessages: dest: "<<thread->getDestinationPath();
        qDebug()<<"DlgErrorMessages: source: "<<thread->getSourcePath();

        if(thread->getDestinationPath() == thread->getSourcePath())
        {
            qDebug()<<"DlgErrorMessages: paths are equal";
            QList<TreeItem*> topitems = ui->treeWidget->getModel()->getTopLevelItems();
            for(int i=0;i<topitems.count();i++)
            {
                TreeItem *item = topitems.value(i);
                item->rename(true);
            }
            on_BtOk_clicked();
            return false;
        }
        return ok;
    }

    if(type==EDIT_MODE || type == FREE_SPACE_MODE)
    {
        ui->treeWidget->showAll();
        ui->lbDestination->setText(ui->treeWidget->getDestination());
        ui->lbDestination->setToolTip(ui->lbDestination->text());
        ui->lbSource->setText(thread->getSourcePath());
        ui->lbSource->setToolTip(ui->lbSource->text());
        ui->label->setText(tr("Edit file copy tree"));
        if(type == FREE_SPACE_MODE)
        {
            QString msg = tr("Not enough (free?) size in destination!\n You need at least: ")+
                          Util::toReadableSize( thread->getTotalSize() -
                                                thread->getCopyDirectory()->getReplaceSize() )+
                          tr("\nOr don't copy some files/folders");
            ui->label->setText(msg);
        }
        return true;
    }
    return false;
}


bool DlgErrorMessages::setErrors(TaskThread *thread, QList<File *> &files, int type)
{
    m_error_type = type;
    ui->label->setText(tr("An error occurred"));
    ui->BtRetry->show();
    ui->BtReplace->hide();
    ui->BtRename->hide();
    adjustSize();
    m_thread = thread;
    ui->treeWidget->setErrorList(thread->getCopyDirectory(),files);
    ui->lbDestination->setText(ui->treeWidget->getDestination());
    ui->lbDestination->setToolTip(ui->lbDestination->text());
    ui->lbSource->setText(thread->getSourcePath());
    ui->lbSource->setToolTip(ui->lbSource->text());
    return true;
}

void DlgErrorMessages::on_BtOk_clicked()
{
    QList<TreeItem*> topitems = ui->treeWidget->getModel()->getTopLevelItems();
    for(int i=0;i<topitems.count();i++)
    {
        TreeItem *item = topitems.value(i);
        if(item->data(COL_EXISTS,Qt::DisplayRole).toString().isEmpty())
            continue;
        if(item->getFile())
        {
            if( !item->getFile()->shouldRetry()
                && item->data(COL_REPLACE,Qt::CheckStateRole) != Qt::Checked
                && item->data(COL_RENAME,Qt::CheckStateRole) != Qt::Checked
                && item->data(COL_IGNORE,Qt::CheckStateRole) != Qt::Checked
                )
            {
                QMessageBox::information(this,tr("Copyhandle not set"),
                                         tr("You need to set the copyhandle for: ")+
                                         item->getFile()->getSourceName());
                return;
            }
        }else
        {
            if( item->data(COL_REPLACE,Qt::CheckStateRole) != Qt::Checked
                && item->data(COL_RENAME,Qt::CheckStateRole) != Qt::Checked
                && item->data(COL_IGNORE,Qt::CheckStateRole) != Qt::Checked
                )
            {
                QMessageBox::information(this,
                                         tr("Copyhandle not set"),
                                         tr("You need to set the copyhandle for: ")
                                         +item->getFolder()->getSourceName());
                return;
            }
        }
    }

    close();
}

void DlgErrorMessages::on_BtCancel_clicked()
{
    m_thread->exit();
    close();
}

void DlgErrorMessages::on_BtIgnore_clicked()
{
    QModelIndexList items = ui->treeWidget->getSelected();

    if(items.count()==0)
    {
        QList<TreeItem*> topitems = ui->treeWidget->getModel()->getTopLevelItems();
        if(topitems.isEmpty())return;
        bool ignore = (topitems.at(0)->data(COL_IGNORE,Qt::CheckStateRole)==0);
        for(int i=0;i<topitems.count();i++)
        {
            TreeItem *item = topitems.value(i);
            if(ignore && item->data(COL_IGNORE,Qt::CheckStateRole) != Qt::Checked )
            {
                if(item->getFile() && m_error_type==ER_COPY)
                {
                    item->getFile()->ignore(true);
                    item->getFile()->setRetry(false);
                }
                if(ui->treeWidget->currentState()==STATE_EXIST){
                    item->ignoreExisting(true);
                }else
                    item->ignore(true);
                ui->treeWidget->getModel()->updateItem(item);
            }
            if(!ignore && item->data(COL_IGNORE,Qt::CheckStateRole) == Qt::Checked )
            {
                if(item->getFile() && m_error_type==ER_COPY)
                {
                    item->getFile()->ignore(false);
                    item->getFile()->setRetry(true);
                }
                if(ui->treeWidget->currentState()==STATE_EXIST){
                    item->ignoreExisting(false);
                }else
                    item->ignore(false);
                ui->treeWidget->getModel()->updateItem(item);
            }
        }
        return;
    }

    bool ignore = (((TreeItem*) items[0].internalPointer())->data(COL_IGNORE,Qt::CheckStateRole)==0);
    for(int i=0;i<items.count();i++)
    {
        TreeItem *item = (TreeItem*) items[i].internalPointer();
        if(ignore && item->data(COL_IGNORE,Qt::CheckStateRole) != Qt::Checked )
        {
            if(item->getFile() && m_error_type==ER_COPY)
            {
                item->getFile()->ignore(true);
                item->getFile()->setRetry(false);
            }
            if(ui->treeWidget->currentState()==STATE_EXIST){
                item->ignoreExisting(true);
            }else
                item->ignore(true);
            ui->treeWidget->getModel()->updateItem(item);
        }
        if(!ignore && item->data(COL_IGNORE,Qt::CheckStateRole) == Qt::Checked )
        {
            if(item->getFile() && m_error_type==ER_COPY)
            {
                item->getFile()->ignore(false);
                item->getFile()->setRetry(true);
            }
            if(ui->treeWidget->currentState()==STATE_EXIST){
                item->ignoreExisting(false);
            }else
                item->ignore(false);
            ui->treeWidget->getModel()->updateItem(item);
        }
    }
}

void DlgErrorMessages::on_BtRename_clicked()
{
    QModelIndexList items = ui->treeWidget->getSelected();

    if(items.count()==0)
    {
        QList<TreeItem*> topitems = ui->treeWidget->getModel()->getTopLevelItems();
        if(topitems.isEmpty())return;
        bool rename = !(topitems.at(0)->data(COL_RENAME).isValid());
        for(int i=0;i<topitems.count();i++)
        {
            TreeItem *item = topitems.value(i);
            if(rename && !item->data(COL_RENAME).isValid() )
            {
                item->rename(true);
                ui->treeWidget->getModel()->updateItem(item);
            }
            if(!rename && item->data(COL_RENAME) != "" )
            {
                item->rename(false);
                ui->treeWidget->getModel()->updateItem(item);
            }
        }
        return;
    }

    bool rename = !(((TreeItem*) items[0].internalPointer())->data(COL_RENAME).isValid());
    for(int i=0;i<items.count();i++)
    {
        TreeItem *item = (TreeItem*) items[i].internalPointer();
        if(rename && !item->data(COL_RENAME).isValid() )
        {
            item->rename(true);
            ui->treeWidget->getModel()->updateItem(item);
        }
        if(!rename && item->data(COL_RENAME) != "" )
        {
            item->rename(false);
            ui->treeWidget->getModel()->updateItem(item);
        }
    }
}

void DlgErrorMessages::on_BtReplace_clicked()
{
    QModelIndexList items = ui->treeWidget->getSelected();
    //ui->treeWidget->treeWidget->blockSignals(true);

    if(items.count()==0)
    {
        QList<TreeItem*> topitems = ui->treeWidget->getModel()->getTopLevelItems();
        if(topitems.isEmpty())return;
        bool replace = (topitems.at(0)->data(COL_REPLACE,Qt::CheckStateRole)==0);
        for(int i=0;i<topitems.count();i++)
        {
            TreeItem *item = topitems.value(i);
            if(replace && item->data(COL_REPLACE,Qt::CheckStateRole) != Qt::Checked )
            {
                item->replace(true);
                ui->treeWidget->getModel()->updateItem(item);
            }
            if(!replace && item->data(COL_REPLACE,Qt::CheckStateRole) == Qt::Checked )
            {
                item->replace(false);
                ui->treeWidget->getModel()->updateItem(item);
            }
        }
        return;
    }

    bool replace = (((TreeItem*) items[0].internalPointer())->data(COL_REPLACE,Qt::CheckStateRole)==0);
    for(int i=0;i<items.count();i++)
    {
        TreeItem *item = (TreeItem*) items[i].internalPointer();
        if(replace && item->data(COL_REPLACE,Qt::CheckStateRole) != Qt::Checked )
        {
            item->replace(true);
            ui->treeWidget->getModel()->updateItem(item);
        }
        if(!replace && item->data(COL_REPLACE,Qt::CheckStateRole) == Qt::Checked )
        {
            item->replace(false);
            ui->treeWidget->getModel()->updateItem(item);
        }
    }
}

void DlgErrorMessages::on_BtRetry_clicked()
{
    QModelIndexList items = ui->treeWidget->getSelected();
    //ui->treeWidget->treeWidget->blockSignals(true);

    if(items.count()==0)
    {
        QList<TreeItem*> topitems = ui->treeWidget->getModel()->getTopLevelItems();
        for(int i=0;i<topitems.count();i++)
        {
            TreeItem *item = topitems.value(i);
            if( item->data(COL_RETRY,Qt::CheckStateRole) != Qt::Checked )
            {
                if(item->getFile())
                {
                    item->getFile()->ignore(false);
                    item->getFile()->setRetry(true);
                }
                ui->treeWidget->getModel()->updateItem(item);
            }
        }
        return;
    }

    for(int i=0;i<items.count();i++)
    {
        TreeItem *item = (TreeItem*) items[i].internalPointer();
        if( item->data(COL_RETRY,Qt::CheckStateRole) != Qt::Checked )
        {
            if(item->getFile())
            {
                item->getFile()->ignore(false);
                item->getFile()->setRetry(true);
            }
            ui->treeWidget->getModel()->updateItem(item);
        }
    }
}


void DlgErrorMessages::closeEvent(QCloseEvent *e)
{
    if(e->spontaneous())
        e->setAccepted(false);
    else
        e->accept();
}
