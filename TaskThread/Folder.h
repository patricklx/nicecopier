#ifndef Folder_h
#define Folder_h

#include "File.h"
#include <QFileInfo>
#include <QVarLengthArray>
#include "treeitem.h"
#include <QIcon>
#include <QList>

class TaskThread;

class Folder{

    public:

        enum CopyHandle{
            COPY_REPLACE,
            COPY_IGNORE,
            COPY_IGNORE_EXISTING,
            COPY_RENAME,
            COPY_NOT_SET
        };

        Folder(TaskThread *copyinfo,Folder* parent, QFileInfo folderinfo=QFileInfo(),bool check_exist = true);
        Folder(TaskThread *copyinfo,QFileInfo source,QFileInfo destination);
        Folder(TaskThread *copyinfo,Folder* parent,QXmlStreamAttributes attributes);
        ~Folder();

        void initialize(TaskThread *copyinfo,Folder* parent, QFileInfo folderinfo=QFileInfo(),bool check_exist = true);

        double getUsedDiskSpace();
        double getExistingSize();
        double getSize();
        bool deleteDestination();
        bool deleteSource();
        size_t getTotalFileCount();
        size_t filesCount();
        size_t getCopiedFilesCount();
        size_t getToBeCopiedFilesCount();

        bool rename(QString new_name = QString());
        void unrename();
        bool exists(bool reCheck = true);

        void ignore(bool ignore);
        void ignoreExisting(bool ignore);
        void replace(bool replace);

        bool isIgnoreExisting();
        bool isReplace();
        bool isIgnore();
        bool isRenamed();
        bool hasCopyStarted();

        QString getDestPath(bool fullPath = true);
        QString getSourcePath(bool fullPath = true);
        QString getDestName();
        QString getSourceName();

        QList<Folder*> getSubFolderList() const;
        QList<File*>   getFileList() const;

        TreeItem* getTreeItem();
        Folder* getParentFolder();

        File *addFile(QFileInfo finfo, bool propagateSize = false);
        void addFile(File *file, bool propagateSize = false);
        void addFolder(Folder *folder, bool propagateSize = false);
        Folder *addFolder(QFileInfo finfo);
        void traverse(QString path);

        bool shouldRetry();
        void setShouldRetry(bool retry);
        void setCopyStarted(bool started=true);

        CopyHandle getCopyHandle();
        void setCopyHandle(CopyHandle handle);

        void setFilesCopied();

        void addSize(double size);
        void addSizeExistingFile(double size,bool updated = false);
        void addSizeReplace(double size);

        void load(QXmlStreamReader &doc);
private:
        bool destExist;
        bool copyStarted;
        bool filesCopied;
        bool retry;
        TreeItem *treeItem;
        TaskThread *copyThreadInfo;
        Folder* parentFolder;

        QByteArray   sourceName;
        QByteArray   destName;
        QByteArray   sourcePath;
        QByteArray   destPath;

        double fsize;
        double fsizeExisting;
        double fsizeReplace;

        CopyHandle copyhandle;

        QList<Folder*> subfolderList;
        QList<File*> fileList;



};


#endif // Folder.h
