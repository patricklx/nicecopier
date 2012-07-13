#ifndef TASKLISTHANDLER_H
#define TASKLISTHANDLER_H

#include <QObject>
#include <QStringList>
#include <extensions/qstringext.h>
class TaskThread;


class TaskListHandler : public QObject
{
    public:
        typedef enum CopyType{
            LOCAL_NETWORK,
            FROM_INET,
            TO_INET,
            DISK_TO_DISK,
            FROM_CDDVD,
            UNKNOWN
        }CopyType;

        typedef enum TaskEnqueue{
            CHECK_SOURCE,
            CHECK_DEST,
            CHECK_BOTH,
            ALWAYS_ENQUEUE
        }TaskEnqueue;

        typedef struct TaskOpList
        {
            QString    destvolume,
                        sourcevolume;
            bool all_running;
            int type;
            QList<void*> thList;

        }TaskOpList;

        TaskListHandler();
        bool add( TaskThread *thread );
        void threadTerminated( TaskThread *thread );
        TaskThread* remove( TaskThread *thread);//return next task
        int m_count;

    private:
        QList<void*>  LocalNetwork,
                        FromINet,
                        ToINet,
                        DiskToDisk,
                        FromCDDVD,
                        OtherMedia;
        QStringList vol;

        bool addToList(QList<void*> &list, TaskThread *thread,int max_tasks );
        CopyType getOperationType(QStringExt source, QStringExt dest);
        TaskThread* removeFromList(QList<void*> &list, TaskThread *thread,int mode );

};

#endif // TASKLISTHANDLER_H
