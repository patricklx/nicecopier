#ifndef FILELIST_H
#define FILELIST_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "TaskThread/mfolder.h"

namespace Ui {
    class filelist;
}

class filelist : public QWidget
{
    Q_OBJECT


public:
    explicit filelist(QWidget *parent = 0);
    ~filelist();

    TreeModel *m_model;
    QTreeView *treeWidget;
    QString destination;
    QList<TreeItem*> top_items;

    int STATE;

    void SetErrorList(mFolder *folder, QList<mFile*> &filelist );

    bool ShowExist();
    void ShowAll();
    void ShowNewer();
    void ShowOlder();
    void SetTopList( QList<TreeItem*> &itemlist, TaskThread *thread );

    QIcon folderIcon;

private:
    Ui::filelist *ui;
    QModelIndexList searchitemlist;
    QString searchtext;
    int searchresult;

public slots:
    void on_treeWidget_customContextMenuRequested(QPoint pos);


private slots:
    void on_treeWidget_expanded(QModelIndex index);
    void on_ckfullpath_stateChanged(int );
    void on_cbtype_activated(int index);
    void on_searchInput_returnPressed();
};

#endif // FILELIST_H
