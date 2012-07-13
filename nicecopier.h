#ifndef NICECOPIER_H
#define NICECOPIER_H

#include <QMainWindow>
#include "mytaskbaricon.h"
#include "tasklisthandler.h"
#include "ncserver.h"
#include "copyinfopanel.h"
#include <QLibrary>
#include <QTimer>
#include "RecentTasks.h"
#include "updater_t.h"

namespace Ui {
    class NiceCopier;
}

class NiceCopier : public QMainWindow
{
        Q_OBJECT


    public:
        explicit NiceCopier(QWidget *parent = 0);
        ~NiceCopier();

    private:
        Ui::NiceCopier *ui;
        TaskListHandler *taskhandler;
        NCServer server;

        QList<CopyInfoPanel*> reserved_panels;
        QList<CopyInfoPanel*> panels;

        MyTaskBarIcon *taskbar;
        Updater updater;


        void closeEvent(QCloseEvent *evt);

    public slots:
        void addTask(QIODevice &info, bool is_new = true);

    private slots:
        void removeTask( CopyInfoPanel *panel );
        void setFirstPanel(CopyInfoPanel *panel);

        void update();
        void taskIconClicked(QSystemTrayIcon::ActivationReason reason);
        void settingsChanged();

        void expandTask( CopyInfoPanel *panel );
        void contractTask( CopyInfoPanel *panel);

        void startTasks();
        void aboutToQuit();
};

#endif // NICECOPIER_H
