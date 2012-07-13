#ifndef MYTASKBARICON_H
#define MYTASKBARICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>

class NiceCopier;

class MyTaskBarIcon : public QSystemTrayIcon
{
        Q_OBJECT

    public:
        MyTaskBarIcon(QWidget *_parent);

    signals:
        void addTask();
        void getUpdate();

    private slots:
        void menuEvent(QAction *act);
        void activated(QSystemTrayIcon::ActivationReason reason);
        void resetTaskBarIcon();

    public slots:
        void newVersionAvailable();
        void askDownload();
        void showMessage(const QString &title, const QString &msg,
                         MessageIcon icon = Information, int msecs = 10000);



    private:
        QMenu menu;
        NiceCopier *parent;
        bool newerVersionAvailable;
        QString lastMessage;
};

#endif // MYTASKBARICON_H
