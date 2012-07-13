#ifndef TASKCOPYQUEUE_H
#define TASKCOPYQUEUE_H

#include <QPoint>
#include <QLinkedList>
#include "Folder.h"
#include "File.h"


class TaskThread;

class TaskCopyQueue : public QObject
{
        Q_OBJECT
    public:
        ~TaskCopyQueue();
        TaskCopyQueue(TaskThread *thread);
        void startCopyQueue();
        void create(TaskThread* thread);
        void setInicialBufferSize(int size){buffersize = size;}
        void setFilePos( QPoint point );
        bool resumeQueueToFile(QString last_file, Folder *f = NULL);

        File* getCurrentFile() const;

        static void setSpeed(int speed);



    private:
        friend class TaskThread;
        File* actFile;
        QPoint filePos;
        TaskThread *copyThreadInfo;

        char *buffer;
        double buffersize;
        double fileSizeToSave;
        bool resume;

        QList<File*> failed_copys;

        bool checkDiskSpace(QString dest);
        void failedCopys_Retry();
        void recursiveCopy(Folder *folder);
        void runCheckSumTest(Folder *folder=NULL);
};

#endif // TASKCOPYQUEUE_H
