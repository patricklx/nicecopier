#ifndef File_h
#define File_h

#include <QString>
#include <QFile>
#include <QObject>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include "treeitem.h"
#include <QIcon>
#include <QXmlStreamAttributes>
#include <extensions/qstringext.h>

class Folder;
class TaskThread;
class File
{
    public:
        enum AgeCompare{OLDER,NEWER,EQUAL};
        enum SizeCompare{SMALLER,BIGGER,SAME};
        enum CopyHandle{COPY_REPLACE,COPY_IGNORE,COPY_RENAME,COPY_NOT_SET};
        enum ChecksumStatus{PASSED,FAILED,NOT_TESTED};

        File(TaskThread *copyinfo,Folder *folder,QFileInfo fileinfo = QFileInfo(),bool check_exists = true);
        File(TaskThread *copyinfo, Folder *folder,QXmlStreamAttributes attributes);
        ~File();
        void initialize(TaskThread *copyinfo,Folder *folder,QFileInfo fileinfo = QFileInfo(),bool check_exists = true);

        double getTargetSize();
        double getSize();
        double getSavedCopiedSize();

        bool startCopyFile();
        bool deleteDest();
        bool deleteSource();
        bool rename(QStringExt new_name = QString());
        void unrename();
        bool exists(bool recheck = false);
        void ignore(bool ignore);
        void ignoreExisting(bool m_ignore);
        void replace(bool replace);
        bool shouldRetry();
        bool shouldSkip();
        void setRetry(bool retry);
        void setSkip(bool skip);

        bool isRenamed();
        bool wasCopied();
        bool hasCopyStarted();
        AgeCompare compareAge();
        SizeCompare compareSize();
        bool isFirst();
        void setFirst();


        static void setSpeed(int speed);

        TreeItem* getTreeItem();
        Folder *getParentFolder();
        CopyHandle getCopyHandle();
        void setCopyHandle(CopyHandle handle);

        QStringExt getDestinationName();
        QStringExt getSourceName();

        QString getSourceError();
        QString getDestinationError();

        void setBuffer(char* buffer, int size);
        int getBufferSize();

        void setDestinationName(QString name);

        bool verifyChecksum();
        double getCopiedSize();

        ChecksumStatus getChecksumStatus();        
        QString getSourceModifiedDate();
        QString getTargetModifiedDate();
private:
        bool retry;
        bool firstInQueue;
        bool skip;

        bool destExist;
        bool copyStarted;
        bool copied;

        ChecksumStatus checkSumStatus;

        AgeCompare destAge;
        SizeCompare destSize;
        CopyHandle copyhandle;


        TreeItem *treeItem;
        TaskThread *copyThreadInfo;
        char *buffer;
        QFile *fSource;
        QFile *fDest;
        Folder *parentFolder;

        QByteArray  fSourceName;
        QByteArray  fDestName;
        QByteArray  derror;
        QByteArray  serror;
        QByteArray  sourceModifiedStr;
        QByteArray  targetModifiedStr;

        size_t buffersize;
        double fsize;
        double targetSize;
        double copiedSize;
        size_t computeBufferSize(int time);
        size_t averageSpeed;

        qint16 sourceCheckSum;


        void saveCopiedSize();
        bool copyFile();
        bool setFileSize();
        bool checkShouldRetry();
        void handleZeroSizeFile();
        bool prepareCopy();
};

#endif // File.h
