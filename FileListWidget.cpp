
#include "ui_filelist.h"
#include "filelistwidget.h"
#include "TaskThread/ReadableSize.h"
#include <QTreeWidget>
#include "TaskThread/Folder.h"
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
#include <QPainter>
#include <string>
#include <iostream>
#include "ncsettings.h"
#include "TaskThread/taskthread.h"

#include <windows.h>
#include <QString>
FileListWidget::FileListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileList)
{
    ui->setupUi(this);
    QMenu *lineEditMenu = new QMenu(ui->searchInput);
    ui->searchInput->setButtonMenu(FancyLineEdit::Left, lineEditMenu);
    ui->searchInput->setButtonVisible(FancyLineEdit::Left, true);
    m_caseSensitiveAction = new QAction(tr("Case Sensitive"), this);
    m_caseSensitiveAction->setIcon(QIcon(QLatin1String(":/icons/find/casesensitively.png")));
    m_caseSensitiveAction->setCheckable(true);
    m_caseSensitiveAction->setChecked(false);
    connect(m_caseSensitiveAction, SIGNAL(triggered(bool)), this, SLOT(setCaseSensitive(bool)));
    lineEditMenu->addAction(m_caseSensitiveAction);

    m_wholeWordAction = new QAction(tr("Whole Words Only"), this);
    m_wholeWordAction->setIcon(QIcon(QLatin1String(":/icons/find/wholewords.png")));
    m_wholeWordAction->setCheckable(true);
    m_wholeWordAction->setChecked(false);
    connect(m_wholeWordAction, SIGNAL(triggered(bool)), this, SLOT(setWholeWord(bool)));
    lineEditMenu->addAction(m_wholeWordAction);

    m_regularExpressionAction = new QAction(tr("Use Regular Expressions"), this);
    m_regularExpressionAction->setIcon(QIcon(QLatin1String(":/icons/find/regexp.png")));
    m_regularExpressionAction->setCheckable(true);
    m_regularExpressionAction->setChecked(false);
    connect(m_regularExpressionAction, SIGNAL(triggered(bool)), this, SLOT(setRegularExpressions(bool)));
    lineEditMenu->addAction(m_regularExpressionAction);
    ui->progressBar->hide();

    model = new TreeModel();
    QItemSelectionModel *m =  ui->treeWidget->selectionModel();
    ui->treeWidget->setModel(model);
    delete m;


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

    filterActive = false;
    filterFlags = FilterNormal;
    updateIcons();
}

FileListWidget::~FileListWidget()
{
    topItems.clear();

    delete ui;
}

void FileListWidget::setTopList( QList<TreeItem*> &itemlist, TaskThread *thread )
{
    this->topItems = itemlist;
    Folder *rootfolder = thread->getCopyDirectory();

    if(thread->getCopyType() == TaskListHandler::LOCAL_NETWORK || thread->getCopyType() == TaskListHandler::FROM_INET)
    {
        model->setOnNetwork();
    }
    destination = rootfolder->getDestPath();
}

void FileListWidget::scrollTo(TreeItem *item)
{
    QModelIndex index = model->getItemIndex(item);
    treeWidget->setCurrentIndex(QModelIndex());
    //Expand all parents so that file->treeitem has the right index
    treeWidget->scrollTo(index,QAbstractItemView::PositionAtTop);
    //scroll to the item
    treeWidget->scrollTo(item->getModelIndex(),QAbstractItemView::PositionAtCenter);
    //select it
    treeWidget->setCurrentIndex(item->getModelIndex());
}

void FileListWidget::updateItem(TreeItem *item)
{
    treeWidget->update( item->getModelIndex() );
}

QModelIndexList FileListWidget::getSelected()
{
    return treeWidget->selectionModel()->selectedRows();
}

void FileListWidget::addTopLevelItem(TreeItem *item)
{
    topItems.append(item);
    model->addTopLevelItem(item);
}

TreeModel* FileListWidget::getModel()
{
    return model;
}

void FileListWidget::clear()
{
    model->removeAllTopItems();
}

States FileListWidget::currentState()
{
    return STATE;
}

QString FileListWidget::getDestination()
{
    return destination;
}


void FileListWidget::setErrorList(Folder *folder, QList<File*> &filelist )
{
    destination = folder->getDestPath();
    ui->cbtype->hide();

    STATE = STATE_ERROR;
    ui->treeWidget->setColumnWidth(COL_IGNORE,200);
    QFileIconProvider ip;
    folderIcon = ip.icon(QFileInfo(folder->getSourcePath()));

    ui->treeWidget->hideColumn(COL_REPLACE);
    ui->treeWidget->hideColumn(COL_RENAME);
    ui->treeWidget->hideColumn(COL_SIZE);
    ui->treeWidget->hideColumn(COL_EXISTS);
    ui->treeWidget->hideColumn(COL_CHECKSUM_STAT);
    ui->treeWidget->showColumn(COL_RETRY);


    for(int i=0; i < filelist.count();i++)
    {
        File *file = filelist[i];
        model->addTopLevelItem(file->getTreeItem());
    }
    ui->treeWidget->resizeColumnToContents(COL_NAME);
}

void FileListWidget::showAll( )
{
    ui->treeWidget->hideColumn(COL_RETRY);

    STATE = STATE_ALL;

    model->setTopItems(this->topItems);

    ui->treeWidget->setColumnWidth(COL_NAME,150);
    QFontMetrics metrics(this->font());

    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->resizeColumnToContents(3);
    ui->treeWidget->resizeColumnToContents(5);
    ui->treeWidget->resizeColumnToContents(6);
    ui->treeWidget->resizeColumnToContents(7);
}

bool FileListWidget::testFunction(TreeItem *item)
{
    QString content = item->data(COL_NAME,Qt::DisplayRole).toString();


    if( filterActive == true ){

        bool ok = false;

        if( filterFlags==FilterNormal ){

            ok = content.contains( filterExpr );
        }

        if( filterFlags&FilterCaseSensitively ){

            ok = content.contains( filterExpr ,Qt::CaseSensitive);
        }

        if( filterFlags&FilterRegularExpression ){

            QRegExp regex( filterExpr );
            ok = content.contains(regex);
        }

        if( !ok )
            return false;
    }

    switch(STATE)
    {
        case STATE_IGNORED:
            return (item->data(COL_IGNORE,Qt::CheckStateRole).toInt() == Qt::Checked);
        break;
        case STATE_REPLACED:
            return (item->data(COL_REPLACE,Qt::CheckStateRole).toInt() == Qt::Checked);
        break;
        case STATE_RENAMED:
            return (!item->data(COL_RENAME,Qt::DisplayRole).toString().isEmpty());
        break;
        case STATE_EXIST:
            return (!item->data(COL_EXISTS,Qt::DisplayRole).toString().isEmpty());
        break;
        case STATE_NEWER:
            return (item->data(COL_EXISTS,Qt::DisplayRole).toString()==tr("Newer"));
        break;
        case STATE_NEW:
            return (item->data(COL_EXISTS,Qt::DisplayRole).toString().isEmpty());
        break;
        case STATE_OLDER:
            return (item->data(COL_EXISTS,Qt::DisplayRole).toString()==tr("Older"));
        break;
        case STATE_SAME:
            return (item->data(COL_EXISTS,Qt::DisplayRole).toString()==tr("Same Date and Size"));
        break;

        default:
            return filterActive;
    }
}

void FileListWidget::setTopList()
{
    QList<TreeItem*> recursiv_list;
    QList<TreeItem*> toplevelitemList;
    recursiv_list = this->topItems;
    if( recursiv_list.isEmpty() )
        return;

    size_t value = 0;
    if(topItems[0]->getFile())
        value = topItems[0]->getFile()->getParentFolder()->filesCount();
    else
        value = topItems[0]->getFolder()->getParentFolder()->filesCount();

    ui->progressBar->setMaximum(value);


    while(!recursiv_list.isEmpty())
    {
        TreeItem *item = recursiv_list[0];
        recursiv_list.removeFirst();
        if(item->childCount()>0)
        {
            //Folder
            int allow = STATE_NEW|STATE_REPLACED|STATE_RENAMED|STATE_IGNORED;
            if(item->getFolder()->exists(false) || STATE&allow || filterActive)
            {
                for(int i=0;i<item->childCount();i++)
                {
                    TreeItem *child = item->child(i);
                    if(child->getFile() && testFunction(child))
                    {
                        toplevelitemList.append(child);
                    }
                    if(child->childCount()>0)
                    {
                        if(filterActive || child->getFolder()->exists(false) || STATE&allow)
                            recursiv_list.append(child);

                        if(STATE==STATE_RENAMED && testFunction(child))
                            toplevelitemList.append(child);
                    }
                }
            }
        }else
        {
            //File
            if(testFunction(item))
            {
                toplevelitemList.append(item);
            }
        }
        value--;
        ui->progressBar->setValue(ui->progressBar->maximum()-value);
        QApplication::processEvents();
    }
    model->setTopItems(toplevelitemList);
}

void FileListWidget::showSame()
{
    STATE = STATE_SAME;
    setTopList();
}

void FileListWidget::showNewer()
{
    STATE = STATE_NEWER;
    setTopList();
}

void FileListWidget::showOlder()
{
    STATE = STATE_OLDER;
    setTopList();
}

void FileListWidget::showIgnored()
{
    STATE = STATE_IGNORED;
    setTopList();
}

void FileListWidget::showReplaced()
{
    STATE = STATE_REPLACED;
    setTopList();
}

void FileListWidget::showRenamed()
{
    STATE = STATE_RENAMED;
    setTopList();
}


void FileListWidget::showNew()
{
    STATE = STATE_NEW;
    setTopList();
}

bool FileListWidget::showExist()
{
    //Be sure that the combobox shows "existing" when showing the "files exists"
    //error message
    this->blockSignals(true);
    ui->cbtype->setCurrentIndex(1);
    this->blockSignals(false);

    int value = this->topItems.count();
    ui->progressBar->setMaximum(value);
    STATE = STATE_EXIST;

    ui->treeWidget->hideColumn(COL_SIZE);
    ui->treeWidget->hideColumn(COL_RETRY);
    ui->treeWidget->hideColumn(COL_CHECKSUM_STAT);

    ui->treeWidget->showColumn(COL_RENAME);
    ui->treeWidget->showColumn(COL_REPLACE);
    ui->treeWidget->showColumn(COL_IGNORE);
    ui->treeWidget->showColumn(COL_EXISTS);

    ui->treeWidget->setColumnWidth(COL_NAME,150);

    QList<TreeItem*> toplevelitemList;

    for(int i=0; i < this->topItems.count();i++)
    {
        TreeItem *treeitem = this->topItems[i];

        if( treeitem->data(COL_EXISTS).isValid())
        {
            toplevelitemList.append(treeitem);
        }
        value--;
        ui->progressBar->setValue(ui->progressBar->maximum()-value);
        QApplication::processEvents();
    }

    model->setTopItems(toplevelitemList);
    ui->treeWidget->update();

    if( model->getTopLevelItems().count() > 0 )
        return true;
    else
        return false;
}

void FileListWidget::on_treeWidget_customContextMenuRequested(QPoint pos)
{
    QModelIndexList items = treeWidget->selectionModel()->selectedRows();
    if(items.count()==0)
        return;

    QMenu menu(this);
    menu.addAction(tr("rename"));
    menu.addAction(tr("ignore existing"));
    menu.addAction(tr("ignore"));

    menu.addAction(tr("replace"));

    if(STATE==STATE_ERROR)
        menu.addAction(tr("retry"));

    if(items.count()==1)
    {
        menu.addSeparator();
        menu.addAction(tr("Open destination"));
        menu.addAction(tr("Open source"));
    }


    QAction *action = menu.exec(mapToGlobal( pos ));
    if(action == NULL)
        return;

    if( action->text() == tr("retry"))
    {
        bool retry = !((TreeItem*) items[0].internalPointer())->getFolder()->shouldRetry();

        foreach(QModelIndex index,items)
        {
            TreeItem *item = (TreeItem*) index.internalPointer();
            item->getFolder()->setShouldRetry(retry);
            ui->treeWidget->update(index);
        }
        return;
    }

    if( action->text() == tr("rename"))
    {
        bool rename = !((TreeItem*) items[0].internalPointer())->data(COL_RENAME,Qt::CheckStateRole).toBool();
        foreach(QModelIndex index,items)
        {
            TreeItem *item = (TreeItem*) index.internalPointer();
            item->rename(rename);
            ui->treeWidget->update(index);
        }
        ui->treeWidget->resizeColumnToContents(COL_RENAME);
    }

    if( action->text() == tr("ignore existing"))
    {
        bool ignore = !((TreeItem*) items[0].internalPointer())->data(COL_IGNORE,Qt::CheckStateRole).toBool();
        foreach(QModelIndex index,items)
        {  
            TreeItem *item = (TreeItem*) index.internalPointer();
            item->ignoreExisting(ignore);
            ui->treeWidget->update(index);
        }
    }

    if( action->text() == tr("ignore"))
    {
        bool ignore = !((TreeItem*) items[0].internalPointer())->data(COL_IGNORE,Qt::CheckStateRole).toBool();
        foreach(QModelIndex index,items)
        {
            TreeItem *item = (TreeItem*) index.internalPointer();
            item->ignore(ignore);
            ui->treeWidget->update(index);
        }
    }
    if( action->text() == "replace")
    {
        bool replace = !((TreeItem*) items[0].internalPointer())->data(COL_REPLACE,Qt::CheckStateRole).toInt();
        foreach(QModelIndex index,items)
        {
            TreeItem *item = (TreeItem*) index.internalPointer();
            item->replace(replace);
            ui->treeWidget->update(index);
        }
    }

    if(action->text() == "Open destination")
    {
        TreeItem *item = (TreeItem*) items[0].internalPointer();

        QString pathIn;

        if(item->getFolder())
        {
            Folder* folder = item->getFolder();
            if(!folder->exists())
                return;
            pathIn = folder->getDestPath(false);
        }else
        {
            File* file = item->getFile();
            if(!file->exists() && file->getCopyHandle()!=File::COPY_RENAME)
                return;
            if( file->hasCopyStarted() ){

                pathIn = file->getDestinationName();

            }else{

                pathIn = file->getParentFolder()->getDestPath() + file->getDestinationName();
            }

        }

        QStringList args;
        args << "/select," << QDir::toNativeSeparators(pathIn);

        QProcess::startDetached("explorer", args);
    }


    if(action->text() == "Open source")
    {
        TreeItem *item = (TreeItem*) items[0].internalPointer();

        QString pathIn;

        if(item->getFolder())
        {
            Folder* folder = item->getFolder();
            pathIn = folder->getSourcePath(false);
            //check if folder exist
            // could be created manually
            QDir dir(pathIn);
            if(!dir.exists())
                return;
        }else
        {
            File* file = item->getFile();
            pathIn = file->getSourceName();
        }

        QStringList args;
        args << "/select," << QDir::toNativeSeparators(pathIn);

        QProcess::startDetached("explorer", args);
    }
}


void FileListWidget::on_treeWidget_expanded(QModelIndex index)
{
    //Neat trick to know if we already handled this item, i had set it to "showIndicator"

    TreeItem *item = (TreeItem*) index.internalPointer();

    if(item->getFolder())
    {
        Folder *folder = item->getFolder();

        int count = 0;
        foreach(Folder *subfolder,folder->getSubFolderList())
        {
            QApplication::processEvents();
            if(NcSettings::isExiting())
                break;

            if( STATE == STATE_EXIST)
            {
                //Only show files and folder that exists in destination
                // Hide the others
                if( subfolder->getTreeItem()->data(COL_EXISTS) == Qt::Checked )
                {
                    ui->treeWidget->setRowHidden(count,folder->getTreeItem()->getModelIndex(),true);
                }
            }else
            {
                //Show items that were hidden by other views
                if(ui->treeWidget->isRowHidden(count,folder->getTreeItem()->getModelIndex()))
                    ui->treeWidget->setRowHidden(count,folder->getTreeItem()->getModelIndex(),false);
            }
            count++;
        }

        count = 0;
        foreach(File *file,folder->getFileList())
        {
            QApplication::processEvents();
            if(NcSettings::isExiting())
                break;

            if( STATE == STATE_EXIST)
            {
                //Only show files and folder that exists in destination
                // Hide the others
                if( file->getTreeItem()->data(COL_EXISTS) == Qt::Checked )
                {
                    ui->treeWidget->setRowHidden(count,file->getTreeItem()->getModelIndex(),true);
                }
            }else
            {
                //Show items that were hidden by other views
                if(ui->treeWidget->isRowHidden(count,file->getTreeItem()->getModelIndex()))
                    ui->treeWidget->setRowHidden(count,file->getTreeItem()->getModelIndex(),false);
            }
            count++;
        }
    }
}


void FileListWidget::on_cbtype_activated(int index)
{
    //remove all items first
    ui->cbtype->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    model->removeAllTopItems();

    switch(index)
    {
        case 0:
            showAll();
        break;
        case 1:
            showExist();
        break;
        case 2:
            showSame();
        break;
        case 3:
            showNewer();
        break;
        case 4:
            showOlder();
        break;
        case 5:
            showNew();
        break;
        case 6:
            showIgnored();
        break;
        case 7:
            showReplaced();
        break;
        case 8:
            showRenamed();
        break;
    }
    ui->cbtype->setEnabled(true);
    ui->progressBar->hide();
}

void FileListWidget::on_ckfullpath_stateChanged(int state)
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

void FileListWidget::on_searchInput_returnPressed()
{
    //remove all items first
    ui->cbtype->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->show();

    model->removeAllTopItems();

    filterExpr = ui->searchInput->text();
    filterActive = true;
    qDebug()<<filterExpr;

    setTopList();

    filterActive = false;

    ui->cbtype->setEnabled(true);
    ui->progressBar->hide();
}

void FileListWidget::setCaseSensitive(bool enabled)
{
    if( enabled )
        this->filterFlags |= FilterCaseSensitively;
    else
        this->filterFlags &= ~FilterCaseSensitively;
    updateIcons();
}

void FileListWidget::setRegularExpressions(bool enabled)
{
    if( enabled )
        this->filterFlags |= FilterRegularExpression;
    else
        this->filterFlags &= ~FilterRegularExpression;
    updateIcons();
}


void FileListWidget::setWholeWord(bool enabled)
{
    if( enabled )
        this->filterFlags |= FilterWholeWords;
    else
        this->filterFlags &= ~FilterWholeWords;
    updateIcons();
}

QPixmap FileListWidget::pixmapForFindFlags(int flags)
{
    static const QPixmap casesensitiveIcon = QPixmap(QLatin1String(":/icons/find/casesensitively.png"));
    static const QPixmap regexpIcon = QPixmap(QLatin1String(":/icons/find/regexp.png"));
    static const QPixmap wholewordsIcon = QPixmap(QLatin1String(":/icons/find/wholewords.png"));
    bool casesensitive = flags & FilterCaseSensitively;
    bool wholewords = flags & FilterWholeWords;
    bool regexp = flags & FilterRegularExpression;
    int width = 0;
    if (casesensitive) width += 6;
    if (wholewords) width += 6;
    if (regexp) width += 6;
    if (width > 0) --width;
    QPixmap pixmap(width, 17);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    int x = 0;

    if (casesensitive) {
        painter.drawPixmap(x - 6, 0, casesensitiveIcon);
        x += 6;
    }
    if (wholewords) {
        painter.drawPixmap(x - 6, 0, wholewordsIcon);
        x += 6;
    }
    if (regexp) {
        painter.drawPixmap(x - 6, 0, regexpIcon);
    }
    return pixmap;
}

void FileListWidget::updateIcons()
{
    int effectiveFlags = filterFlags;
    bool casesensitive = effectiveFlags & FilterCaseSensitively;
    bool wholewords = effectiveFlags & FilterWholeWords;
    bool regexp = effectiveFlags & FilterRegularExpression;
    if (!casesensitive && !wholewords && !regexp) {
        QPixmap pixmap(17, 17);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        const QPixmap mag = QPixmap(QLatin1String(":/icons/find/magnifier.png"));
        painter.drawPixmap(0, (pixmap.height() - mag.height()) / 2, mag);
        ui->searchInput->setButtonPixmap(FancyLineEdit::Left, pixmap);
    } else {
        ui->searchInput->setButtonPixmap(FancyLineEdit::Left,
                                       pixmapForFindFlags(effectiveFlags));
    }
}
