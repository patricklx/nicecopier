#ifndef RECENT_TASKS_T_H
#define RECENT_TASKS_T_H

#include <QStringList>
#include <QString>

class RecentTasks
{
public:
    static void initialize();

    static QStringList getRecentTaskList();
    static QStringList getPinnedTaskList();
    static QString getTask(QString task);


    static void addRecentTask(QString content);
    static void pinnRecentTask(QString task);
    static void unPinnRecentTask(QString task);
    static bool rename(QString old,QString _new,bool listOnly = false);
    static void setTaskFirst(QString task);
    static void setTaskDate(QString task);
};

#endif // RECENT_TASKS_T_H
