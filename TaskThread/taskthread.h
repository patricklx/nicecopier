#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QThread>
#include <QMutex>
#include <QStringList>
#include "taskcopyqueue.h"
#include "taskpreparer.h"
#include "Folder.h"
#include <QXmlStreamWriter>
#include <QXmlReader>
#include <QMessageBox>
#include <QLocalSocket>
#include "tasklisthandler.h"
#include "extensions/qstringext.h"
#include "extensions/qxmlstreamattributesext.h"
#include "extensions/filelist.h"


class TaskThread : public QThread
{
        Q_OBJECT

    public:

        enum CopyOperation{
            OpMove,
            OpCopy
        };

        enum TaskState{
                NotStarted,
                Preparing,
                PrepareDone,
                Copy,
                CopyDone,
                VerifyChecksum,
                RemovingSource,
                RemovingTarget
        };

        enum Config{
            ParseOnly=true,
            ParseAndCopy=false
        };
        TaskThread(QIODevice &xml_device,  Config config = ParseAndCopy);
        ~TaskThread();

        int getTaskId();

        enum{
            NormalExit=false,
            ExitNow=true
        };
        void exit(bool force = NormalExit);

        void pause();
        void resume();


        bool hasStarted();

        bool isPaused();
        bool isPreparing();
        bool isExiting();

        bool isDeleteDestinationScheduled();
        bool isEditEnabled();
        void disableEdit();

        void scheduleDeleteDestination();

        double getActFileSize();
        double getActFileCpySize();
        QString getActFileError();
        QString getSourcePath();
        QString getDestinationPath();
        Folder* getCopyDirectory();
        CopyOperation getCopyOperation();
        File* getCurrentFile() const;
        QString getCurrentFileName();
        QString getCurrentDestinationFolder();
        QString getCurrentSourceFolder();

        QString getSourceVolume();
        QString getTargetVolume();

        int isFileLoaded();

        void setCopyType( TaskListHandler::CopyType type );
        void setCopyOperationType(CopyOperation type){copyOperationType=type;}
        void setSpeed(int speed);

        double getTotalSize();
        double getTotalFiles();
        double getTotalCopiedSize();
        double getFileSizeCopied();
        QList<QStringExt> getSourceList();

        void updateTotalFiles();

        TaskListHandler::CopyType getCopyType()
        {
            return copyType;
        }

        void aquireMutex();
        void releaseMutex();

        bool testDestroy();

        void setCurrentFile( QString file );

        void sendMessage(QString msg,int *answer = NULL,int buttons = QMessageBox::Ok){
            emit message(msg,answer,buttons);
        }

        void sendPrepareErrorMessage(int error){
            emit prepareErrorMessage(this,error);
        }

        void sendCopyErrorMessage(FileList list,int error){
            emit copyErrorMessage(this,list,error);
        }

        void setCurrentDirectory(QString sourcePath,QString dest);
        void addFileCount( int count );

        void updateFileSizeSave();

        void setFilesIgnored();

        void addTotalSize(double size);
        void addSizeDone( double size);
        void addCopiedSize(double size);

        void setTotalSize(double size);

        bool shouldVerifyChecksum();
        void scheduleMd5Check(bool enable);

        TaskState getState();
private:

        friend class TaskCopyQueue;
        CopyOperation copyOperationType;
        TaskState state;

        QStringExt destinationPath;
        QStringExt sourcePath;
        QStringExt destVolume;
        QStringExt sourceVolume;
        QStringExt currentSourceDir;
        QStringExt currentDestDir;


        /*Task properties*/
        //Indicated that the application is exiting
        bool forceExit;
        //Indicated that the task was paused before the application existed
        bool prepareDone;
        TaskListHandler::CopyType copyType;
        int taskNr;

        bool started;
        bool filesIgnored;
        bool deleteDest;

        bool checkMd5;

        Folder *directory;
        TaskCopyQueue fileCopyQueue;
        TaskPreparer prepareFiles;
        /* Copy status variabled*/

        double totalFiles;
        double totalSize;
        double totalCpSize;
        double totalSizeDone;
        double fileSizeToSave;

        QStringExt resumeToThisFile;
        QString currentFileCopy;

        /*Task variables*/

        //Task should end
        bool shouldExit;
        bool paused;
        bool edit;//edit after preparing

        QList<QStringExt> sourceList;
        QMutex infoMutex;

        int loadFileSuccess;


        void xmlSaveFiles(QXmlStreamWriterExt &doc,Folder *folder);
        void loadFiles( QXmlStreamReader &doc , Folder *folder);
        void setItemPosition(QString item,int startIndex);
        QPoint desktopPoint;
        void saveAllInfo();



        void run();

        void deleteSource();
        void deleteDestination();
        int loadFromXML( QIODevice &xml_device, bool no_save = false );
        QString getRealTargetPath(QString target);

    signals:
        void starting();
        void prepared();
        void copyDone();

        void message(QString msg,int *answer,int buttons);
        void prepareErrorMessage(TaskThread* thread,int error);
        void copyErrorMessage(TaskThread *thread,FileList list,int error);
};

#endif // TASKTHREAD_H
