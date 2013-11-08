#ifndef MFOLDER_H
#define MFOLDER_H

#include "mFile.h"
#include <QFileInfo>
#include <QVarLengthArray>
#include "treeitem.h"
#include <QIcon>

class TaskThread;

class mFolder
{

    public:
        bool dest_exist;
        bool started_copy;
        bool files_copied;
        bool retry;
        TreeItem *treeitem;
        TaskThread *CopyThreadInfo;
        mFolder *parent_folder;

        std::wstring   source_name;
        std::wstring   dest_name;
        std::wstring   source_path;
        std::wstring   dest_path;

        double fsize;
        double fsize_existing;
        double fsize_replace;

        enum enumCOPYHANDLE{COPY_REPLACE,COPY_IGNORE,COPY_IGNORE_EXISTING,COPY_RENAME,COPY_NOT_SET};

        enumCOPYHANDLE copyhandle;

        QList<mFolder*> subfolder_list;
        QList<mFile*> file_list;


	mFolder(TaskThread *copyinfo,mFolder *parent, QFileInfo folderinfo,bool check_exist = true);
        ~mFolder();

        void Traverse(TaskThread *info, QString path);
        void addSize(double size);
        void addSizeExistingFile(double size,bool updated = false);
        void addSizeReplace(double size);
        double getReplaceSize();

        bool DeleteDest();
        bool DeleteSource();
        size_t getTotalFileCount();
	size_t filesCount();
        size_t getCopiedFilesCount();
        size_t getToBeCopiedFilesCount();

        bool rename(QString new_name = QString());
        void unrename();
        bool exists();

        void ignore(bool ignore);
        void ignoreExisting(bool ignore);
        void replace(bool replace);

        bool isIgnoreExisting();
        bool isReplace();
        bool isIgnore();

        void update(mFile *file,int type);
        void update(mFolder *file,int type);

        QString getDestPath();
        QString getSourcePath();
};


#endif // MFOLDER_H
