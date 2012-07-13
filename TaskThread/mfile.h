#ifndef MFILE_H
#define MFILE_H

#include <QString>
#include <QFile>
#include <QObject>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include "treeitem.h"
#include <QIcon>

class mFolder;
class TaskThread;
class mFile
{
    public:
        enum enumAge{OLDER,NEWER,EQUAL};
        enum enumCOPYHANDLE{COPY_REPLACE,COPY_IGNORE,COPY_RENAME,COPY_NOT_SET};

        bool retry;
        bool IsFirstInQueue;
        bool skip;

        bool dest_exist;
        bool copy_started;
        bool copied;

        int TaskId;

        enumAge destAge;
        enumCOPYHANDLE copyhandle;


        TreeItem *treeitem;
        TaskThread *CopyThreadInfo;
        char *buffer;
        QFile *fSource;
        QFile *fDest;
        mFolder *parent_folder;

	QByteArray  fSourceName;
	QByteArray  fDestName;
	QByteArray  derror;
	QByteArray  serror;

        size_t buffersize;
        double fsize;  
        double mCopiedSize;

	mFile(TaskThread *copyinfo,mFolder *folder,QFileInfo fileinfo = QFileInfo(),bool check_exists = true);
        bool StartCopyFile();
        bool DeleteDest();
        bool DeleteSource();
        bool rename(QString new_name = QString());
        void unrename();
        bool exists();
        void ignore(bool ignore);
        void ignoreExisting(bool m_ignore);
        void replace(bool replace);
        virtual ~mFile();

        static void setSpeed(int speed);

    private:

        size_t Calc_BfSize(int time);
        size_t average_speed;
        bool ShouldRetry();
        void SaveCopiedSize();
        bool mCopyFile(QFile* fsource, QFile* fdest);
        bool SetFileSize();
        double GetCopiedSize();
};

#endif // MFILE_H
