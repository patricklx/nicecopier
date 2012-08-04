#ifndef TREEITEM_H
#define TREEITEM_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <QFileIconProvider>

class File;
class Folder;

enum States{
    STATE_EXIST     =0x0001,
    STATE_ALL       =0x0002,
    STATE_ERROR     =0x0004,
    STATE_NEWER     =0x0008,
    STATE_OLDER     =0x0010,
    STATE_SAME      =0x0020,
    STATE_SAMEDATE  =0x0040,
    STATE_IGNORED   =0x0080,
    STATE_REPLACED  =0x0100,
    STATE_RENAMED   =0x0200,
    STATE_NEW       =0x0400,
    STATE_UNDEF     =0x0800
};

enum Colums{
    COL_NAME,
    COL_PATH,
    COL_IGNORE,
    COL_REPLACE,
    COL_RENAME,
    COL_SIZE,
    COL_EXISTS,
    COL_CHECKSUM_STAT,
    COL_RETRY,
    COL_DEBUG
};


class TreeItem
{
    public:


        TreeItem();
        TreeItem(File &file, TreeItem *parent = 0);
        TreeItem(Folder &folder, TreeItem *parent = 0);
        ~TreeItem();

        QModelIndex getModelIndex() const;
        void setModelIndex(QModelIndex index);
        int getRow();
        void setRowIndex(int modelIndex);
        QVariant data(int column,int role = Qt::DisplayRole);
        void rename(bool rename,QString name = QString(""));
        void ignore(bool ignore);
        void ignoreExisting(bool ignore);
        void replace(bool replace);

        File* getFile();
        Folder* getFolder();
        TreeItem *parent();
        TreeItem *child(int getRow);
        int childCount() const;
        int columnCount() const;

        void setTopLevelItem(bool isTopLvel);

        bool isTopLevelItem();

    private:
        bool topLevelItem;
        TreeItem *parentItem;
        File *file;
        Folder *folder;
        QModelIndex modelIndex;
        int rowIndex;
};

class TreeModel : public QAbstractItemModel
{
        Q_OBJECT


    public:
        TreeModel();
        ~TreeModel();

        QList<TreeItem*> getTopLevelItems();

        void setTopItems(QList<TreeItem*> &top_items);
        void addTopLevelItem( TreeItem *item);

        void removeAllTopItems();
        void removeTopLevelItem(TreeItem* item);

        void updateItem(TreeItem *item);

        void sort(int column, Qt::SortOrder order);


        QModelIndex getItemIndex(TreeItem *item)
        {
            return createIndex(item->getRow(),0,item);
        }

        void setBeginInsertRows(TreeItem *item,int row)
        {
            QModelIndex index = createIndex(item->getModelIndex().row(),0,item);
            beginInsertRows(index,row,row);
        }

        void setEndInsertRows()
        {
            endInsertRows();
        }


        void setOnNetwork(bool isOnNetwork = true)
        {
            onNetwork = isOnNetwork;
        }

        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const;

    private:

        bool onNetwork;
        QList<QVariant> headers;
        QList<TreeItem*> topLevelItems;
        QFileIconProvider iconProvider;

        QVariant data(const QModelIndex &index, int role) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role);
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;

        QModelIndex parent(const QModelIndex &index) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;




        void setupModelData(const QStringList &lines, TreeItem *parent);


};

#endif // TREEITEM_H
