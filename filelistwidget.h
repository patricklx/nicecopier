#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "TaskThread/Folder.h"
#include <ui_filelist.h>

namespace Ui {
    class FileList;
}

class FileListWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit FileListWidget(QWidget *parent = 0);
        ~FileListWidget();

        void setErrorList(Folder *folder, QList<File*> &FileListWidget );

        bool testFunction(TreeItem *item);
        void setTopList();

        bool showExist();
        void showAll();
        void showNewer();
        void showSame();
        void showNew();
        void showOlder();
        void showIgnored();
        void showReplaced();
        void showRenamed();
        void showBigger();
        void showSmaller();
        void setTopList( QList<TreeItem*> &itemlist, TaskThread *thread );

        void scrollTo(TreeItem* item);
        void updateItem(TreeItem *item);
        QModelIndexList getSelected();

        void addTopLevelItem(TreeItem *item);

        TreeModel* getModel();

        void clear();

        States currentState();

        QString getDestination();

private:
        typedef enum FilterFlag {
            FilterNormal = 0,
            FilterCaseSensitively = 0x1,
            FilterWholeWords = 0x2,
            FilterRegularExpression = 0x4
        }FilterFlag;

        enum Compare{
            Name,
            Source,
            Target,
            Result
        };

        Ui::FileList *ui;
        QList<TreeItem*> topLevelItemsTempSearch;
        QString searchtext;
        int searchresult;
        TreeModel* model;
        QTreeView* treeWidget;
        QString destination;
        QList<TreeItem*> topItems;
        QIcon folderIcon;
        States state;
        bool filterActive;
        QString filterExpr;
        int filterFlags;

        QAction *m_caseSensitiveAction;
        QAction *m_wholeWordAction;
        QAction *m_regularExpressionAction;

        void updateIcons();
        QPixmap pixmapForFindFlags(int flags);

    public slots:
        void on_treeWidget_customContextMenuRequested(QPoint pos);

    private slots:
        void on_treeWidget_expanded(QModelIndex index);
        void on_ckfullpath_stateChanged(int );
        void on_cbtype_activated(int index);
        void on_searchInput_returnPressed();
        void setCaseSensitive(bool enabled);
        void setRegularExpressions(bool enabled);
        void setWholeWord(bool enabled);
};

#endif // FILELISTWIDGET_H
