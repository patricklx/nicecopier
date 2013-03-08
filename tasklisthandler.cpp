
#include <QtNetwork/QHostAddress>
#include <windows.h>
#include <ncsettings.h>
#include <QHostInfo>
#include <QTimer>

#include "tasklisthandler.h"
#include "TaskThread/taskthread.h"

TaskListHandler::TaskListHandler()
{
    m_count = 0;
}

TaskThread* TaskListHandler::removeFromList(QList<void*> &list, TaskThread *thread,int mode )
{
    int i = 0, count = list.count();
    TaskOpList *th = NULL;

    while( i < count)
    {
        th = (TaskOpList*) list[i];
        if( mode == ALWAYS_ENQUEUE )
        {
            if( !th->thList.contains( thread ) )
                return NULL;
            th->thList.removeOne( thread );
            break;
        }
        /*Only Enqeueu if source and destination are the same*/
        if( mode == CHECK_BOTH && (th->destvolume == thread->getTargetVolume() || th->sourcevolume == thread->getSourceVolume()
                                   || th->sourcevolume == thread->getTargetVolume() || th->destvolume == thread->getSourceVolume()))
        {
            if( !th->thList.contains( thread ) )
                return NULL;
            th->thList.removeOne( thread );
            break;
        }
        /*Only Enqeueu if destination is the same*/
        if( mode == CHECK_DEST && (th->destvolume == thread->getTargetVolume() || th->destvolume == thread->getSourceVolume() ))
        {
            if( !th->thList.contains( thread ) )
                return NULL;
            th->thList.removeOne( thread );
            break;
        }
        /*Only Enqeueu if source is the same*/
        if( mode == CHECK_SOURCE && (th->sourcevolume == thread->getSourceVolume() || th->sourcevolume == thread->getTargetVolume() ))
        {
            if( !th->thList.contains( thread ) )
                return NULL;
            th->thList.removeOne( thread );
            break;
        }
        i++;
    }

    if( th != NULL && th->thList.count() == 0 )
    {
        list.removeOne( th );
        delete th;
        return NULL;
    }

    if( th != NULL && th->thList.count() > 0 )
        return (TaskThread*)th->thList[0];

    return NULL;
}

TaskThread* TaskListHandler::remove( TaskThread *thread )//return next task
{
    m_count--;
    int type = thread->getCopyType();
    switch( type )
    {
        case LOCAL_NETWORK:
        return removeFromList( LocalNetwork, thread, ALWAYS_ENQUEUE );
        break;
        case FROM_INET:
        return removeFromList( FromINet, thread, CHECK_SOURCE );
        break;
        case TO_INET:
        return removeFromList( ToINet, thread, ALWAYS_ENQUEUE );
        break;
        case DISK_TO_DISK:
        return removeFromList( DiskToDisk, thread, CHECK_BOTH );
        break;
        case FROM_CDDVD:
        return removeFromList( FromCDDVD, thread, CHECK_SOURCE);
        case -1:
        {
            return removeFromList( OtherMedia, thread, CHECK_SOURCE );
        }
    }
    return NULL;
}


void TaskListHandler::threadTerminated( TaskThread *thread )
{
    TaskThread *next = remove( thread );
    if( next!=NULL && !next->hasStarted())
    {
        QTimer::singleShot(1,next,SLOT(start()));
    }
}

QString getIP4AdressByName(QString address_name)
{
    QHostInfo info = QHostInfo::fromName(address_name);
    if(info.error()==QHostInfo::NoError)
    {
        for(int i=0;i<info.addresses().count();i++)
        {
            QHostAddress address = info.addresses().at(i);
            if(address.protocol()==QAbstractSocket::IPv4Protocol)
                return address.toString();
        }
    }
    return "";
}


TaskListHandler::CopyType TaskListHandler::getOperationType(QStringExt source, QStringExt dest)
{
    qDebug()<<"TaskListHandler: Operation Type";
    qDebug()<<"TaskListHandler: "<<dest;
    qDebug()<<"TaskListHandler: "<<source;
    if( dest.startsWith("//") )
    {
        QStringExt IpAdress = getIP4AdressByName(dest.afterFirst("//"));
        if(!IpAdress.isEmpty())
        {
            QStringExt ip_part = IpAdress.beforeFirst('.');
            QStringExt ip_part2 = IpAdress.afterFirst('.');
            ip_part2 = ip_part2.beforeFirst('.');
            if( ip_part == "192" && ip_part2 == "168" )
                return LOCAL_NETWORK;
            else
                return TO_INET;
        }
    }

    if( source.startsWith("//") )
    {
        QStringExt IpAdress = getIP4AdressByName(source.afterFirst("//"));
        if(!IpAdress.isEmpty())
        {
            QStringExt ip_part = IpAdress.beforeFirst('.');
            QStringExt ip_part2 = IpAdress.afterFirst('.');
            ip_part2 = ip_part2.beforeFirst('.');
            if( ip_part == "192" && ip_part2 == "168" )
                return LOCAL_NETWORK;
            else
                return FROM_INET;
        }
    }

    if(GetDriveType(source.toStdWString().c_str()) == DRIVE_REMOTE)
        return FROM_INET;
    if(GetDriveType(dest.toStdWString().c_str()) == DRIVE_REMOTE)
        return TO_INET;


    if( !dest.startsWith("//") && !source.startsWith("//"))
    {
        int volume = GetDriveType(source.toStdWString().c_str());
        if( volume == DRIVE_CDROM  )
            return FROM_CDDVD;
        return DISK_TO_DISK;
    }
    return UNKNOWN;
}


bool TaskListHandler::addToList(QList<void*> &list, TaskThread *thread,int mode )
{
    bool queue_exist = false;
    int i = 0, count = list.count();
    while( i < count)
    {
        TaskOpList *th = (TaskOpList*) list[i];
        if( mode == ALWAYS_ENQUEUE )
        {
            th->thList.append( thread );
            queue_exist = true;
            break;
        }
        if( mode == CHECK_BOTH && (th->destvolume == thread->getTargetVolume() || th->sourcevolume == thread->getSourceVolume()
                                   || th->sourcevolume == thread->getTargetVolume() || th->destvolume == thread->getSourceVolume()))
        {
            th->thList.append( thread );
            queue_exist = true;
            break;
        }
        if( mode == CHECK_DEST && (th->destvolume == thread->getTargetVolume() || th->destvolume == thread->getSourceVolume()))
        {
            th->thList.append( thread );
            queue_exist = true;
            break;
        }
        if( mode == CHECK_SOURCE && (th->sourcevolume == thread->getSourceVolume() || th->sourcevolume == thread->getTargetVolume() ))
        {
            th->thList.append( thread );
            queue_exist = true;
            break;
        }
        i++;
    }
    if( !queue_exist )
    {
        TaskOpList *th = new TaskOpList;
        th->destvolume = thread->getTargetVolume();
        th->sourcevolume = thread->getSourceVolume();
        list.append( th );
        th->thList.append( thread );
        return true;
    }
    return false;
}

bool TaskListHandler::add( TaskThread *thread )
{
    QString source = thread->getSourceVolume();
    QString dest = thread->getTargetVolume();
    m_count++;

    CopyType type = thread->getCopyType();
    if( type == UNKNOWN )
    {
        type = getOperationType( source, dest );
        thread->setCopyType( type );
    }

    bool start = false;
    switch( type )
    {
        case LOCAL_NETWORK:
            if( addToList( LocalNetwork, thread, ALWAYS_ENQUEUE ) )
                start = true;
            qDebug()<<"TaskListHandler: " << "LOCAL_NETWORK";

        break;
        case FROM_INET:
            if( addToList( FromINet, thread, CHECK_SOURCE ) )
            {
                start = true;
            }
            qDebug()<<"TaskListHandler: "<< "FROM_INET";
        break;
        case TO_INET:
            if( addToList( ToINet, thread,  CHECK_DEST))
            {
                start = true;
            }
            qDebug()<<"TaskListHandler: "<< "TO_INET";
        break;
        case DISK_TO_DISK:
            if( addToList( DiskToDisk, thread, CHECK_BOTH ) )
            {
                start = true;
            }
            qDebug()<<"TaskListHandler: "<< "DISK_TO_DISK";
        break;
        case FROM_CDDVD:
            if( addToList( FromCDDVD, thread, CHECK_SOURCE ) )
            {
                start = true;
            }
            qDebug()<<"TaskListHandler: "<< "FROM_CDDVD";
        case UNKNOWN:
        {
            if( addToList( OtherMedia, thread, CHECK_BOTH ) )
            {
                start = true;
            }
            qDebug()<<"TaskListHandler: "<< "UNKNOWN";
        }
    }

    if(start && NcSettings::getValue<bool>(NcSettings::START_ON_CREATE) && !thread->isPaused() )
    {
        thread->start();
        return true;
    }
    return false;
}
