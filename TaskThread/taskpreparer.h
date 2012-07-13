#ifndef TASKPREPARER_H
#define TASKPREPARER_H

#include <QFileInfo>
#include <QPoint>
#include "Folder.h"

class TaskThread;


class TaskPreparer : public QObject
{
        Q_OBJECT
    public:
        TaskPreparer();
        bool prepareCopy();
        void create(TaskThread* thread, QString source, QString dest);
        void setDirectoryPos( QPoint point ){
            directoryPos = point;
        }

        void deleteDest(Folder *folder);

    private:
        QFileInfo sourceDir;
        QPoint directoryPos;
        void checkDestFreeSize();
        QString destination;
        TaskThread *copyThreadInfo;
        Folder *directory;
        QList<QStringExt> sourceList;
};

#endif // TASKPREPARER_H
