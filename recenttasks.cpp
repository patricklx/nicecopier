#include "RecentTasks.h"
#include <QList>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <QTime>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include "ncsettings.h"

void RecentTasks::initialize()
{
    QDir dir;
    if(!dir.exists("recent_tasks"))
        dir.mkdir("recent_tasks");

    qsrand(QDateTime::currentMSecsSinceEpoch());
}


QString RecentTasks::getTask(QString task)
{
    QFile file("recent_tasks/"+task);
    if(file.open(QIODevice::ReadOnly))
    {
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	return stream.readAll();
    }
    return "";
}

QStringList RecentTasks::getPinnedTaskList()
{
    QFile file("recent_tasks/pinnedlist.txt");
    if(file.open(QIODevice::ReadOnly))
    {
	QStringList list;
	QTextStream stream(&file);
	while(!stream.atEnd())
	{
	    list.append( stream.readLine() );
	}
	return list;
    }
    return QStringList();
}

QStringList RecentTasks::getRecentTaskList()
{
    QFile file("recent_tasks/recentlist.txt");
    if(file.open(QIODevice::ReadOnly))
    {
	QStringList list;
	QTextStream stream(&file);
	while(!stream.atEnd())
	{
	    list.append( stream.readLine() );
	}
	return list;
    }
    return QStringList();
}


void RecentTasks::setTaskFirst(QString task)
{
    QStringList list = getRecentTaskList();
    if(list.contains(task))
    {
        list.removeOne(task);
        list.insert(0,task);
        QFile file("recent_tasks/recentlist.txt");
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            stream << list.join("\n");
        }
    }
    setTaskDate(task);
}


void RecentTasks::addRecentTask(QString content)
{
    QStringList list = getRecentTaskList();

    QDomDocument doc;
    if( !doc.setContent(content) )
	return;

    doc.firstChildElement().setAttribute("ID",0);
    doc.firstChildElement().setAttribute("STARTED",0);
    doc.firstChildElement().setAttribute("PAUSED",0);
    doc.firstChildElement().setAttribute("DATE",QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate));
    doc.firstChildElement().setAttribute("OP","COPY");

    content = doc.toString();

    QString taskname;
    while(1)
    {
    taskname = "task_%1";
	taskname = taskname.arg(qrand());
	QFile file("recent_tasks/"+taskname);
	if(!file.exists())
	    break;
    }

    list.insert(0,taskname);

    if(list.count()>NcSettings::getValue<int>(NcSettings::MAX_RECENT_TASKS))
    {
	QString oldTask = list.takeLast();
	QFile::remove("recent_tasks/"+oldTask);
    }

    QFile file("recent_tasks/"+taskname);

    if( !file.open(QIODevice::WriteOnly) )
	return;
    file.write(content.toUtf8());


    QFile file2("recent_tasks/recentlist.txt");
    if(!file2.open(QIODevice::WriteOnly))
	return;
    QTextStream stream(&file2);
    stream << list.join("\n");
}

void RecentTasks::pinnRecentTask(QString task)
{
    QFile file("recent_tasks/pinnedlist.txt");
    QStringList list = getPinnedTaskList();
    if(file.open(QIODevice::WriteOnly))
    {
	list.insert(0,task);
	QTextStream stream(&file);
	stream << list.join("\n");

	list = getRecentTaskList();
	list.removeOne(task);
	QFile file2("recent_tasks/recentlist.txt");
	if(!file2.open(QIODevice::WriteOnly))
	    return;
	QTextStream stream2(&file2);
	stream2 << list.join("\n");
    }
}

void RecentTasks::unPinnRecentTask(QString task)
{
    QStringList list = getPinnedTaskList();
    if(list.count()==0)
	return;

    list.removeOne(task);

    QFile file("recent_tasks/pinnedlist.txt");
    if( file.open(QIODevice::WriteOnly) )
    {
	QTextStream stream(&file);
	stream << list.join("\n");
	file.close();
    }

    list = getRecentTaskList();
    list.insert(0,task);
    if(list.count()>NcSettings::getValue<int>(NcSettings::MAX_RECENT_TASKS))
    {
	QString oldTask = list.takeLast();
	QFile::remove("recent_tasks/"+oldTask);
    }
    QFile file2("recent_tasks/recentlist.txt");
    if( file2.open(QIODevice::WriteOnly) )
    {
	QTextStream stream(&file2);
	stream << list.join("\n");
	file2.close();
    }
}

bool RecentTasks::rename(QString old, QString _new,bool listOnly)
{
    QStringList list = getPinnedTaskList();
    for(int i=0;i<list.count();i++)
    {
	if(list[i]==old)
	{
	    list[i]=_new;
	    QFile file("recent_tasks/pinnedlist.txt");
	    if(!file.open(QIODevice::WriteOnly))
		return false;
	    QTextStream stream2(&file);
	    stream2 << list.join("\n");
	    break;
	}
    }

    list = getRecentTaskList();
    for(int i=0;i<list.count();i++)
    {
	if(list[i]==old)
	{
	    list[i]=_new;
	    QFile file("recent_tasks/recentlist.txt");
	    if(!file.open(QIODevice::WriteOnly))
		return false;
	    QTextStream stream2(&file);
	    stream2 << list.join("\n");
	    break;
	}
    }
    if(listOnly)
        return false;

    bool renameOK = QFile::rename("recent_tasks/"+old,"recent_tasks/"+_new);
    if(!renameOK)
    {
        rename(_new,old,true);
    }
    return renameOK;
}


void RecentTasks::setTaskDate(QString task)
{
    QFile file("recent_tasks/"+task);
    if( !file.open(QFile::ReadOnly) )
        return;

    QDomDocument doc;
    doc.setContent(&file);

    QDateTime date = QDateTime::currentDateTime();
    doc.firstChildElement().setAttribute("DATE",date.toString(Qt::SystemLocaleShortDate));
    QString output = doc.toString();

    file.close();
    if( !file.open(QFile::WriteOnly) )
        return;
    file.write(output.toUtf8());
}
