#ifndef COPYINFOPANEL_H
#define COPYINFOPANEL_H

#include <QFrame>
#include "TaskThread/taskthread.h"
#include "tasklisthandler.h"
#include "extensions/qstringext.h"
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QCloseEvent>

namespace Ui {
    class copyInfoPanel;
}

class CopyInfoPanel : public QFrame
{
        Q_OBJECT
    public:
        explicit CopyInfoPanel(QWidget *parent, TaskListHandler* listHandler);
        ~CopyInfoPanel();

        bool createTask( QIODevice &info );
        int getTaskId();
        bool isTaskValid();
        bool isStoppedByUser();
        QString getSourceListMsg();


        void dragEnterEvent(QDragEnterEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *);
        void paintEvent(QPaintEvent *e);
signals:
        void finishedEvent(CopyInfoPanel *panel);
        void startEvent(CopyInfoPanel *panel);
        void expand(CopyInfoPanel *panel);
        void contract(CopyInfoPanel *panel);
        void createNewTask(QIODevice &info);

    private:
        Ui::copyInfoPanel *ui;
        QTimer updateTimer;
        int update_filedone;
        QString fileDoneUpdate;
        QTime watchTime;
        QTime watchSpeed;
        QTime watchSize;
        QTime adjustSpeed;
        QTime watchfiledone;
        double  intermed_size,
        intermed_speed,
        average_speed;
        QPushButton *button;
        void resizeEvent(QResizeEvent *evt);
        void closeEvent(QCloseEvent *evt);
        void setDefaultHandle(TreeItem *treeitem);

        TaskThread *task;
        TaskListHandler *copyTasksHandler;
        bool startedByUser;
        bool stoppedByUser;
        bool error_message;


        QList<QStringExt> sourceList;
        QString source,destination;

        void preventSleep();
        void updateSize( );
        void updateFileDone();
        void updateSpeed( int time );
        void updateTime(  );
        void calcMidSpeed( int time );
        void updateSearchfiles( );
        void changeEvent(QEvent *e);
        void dropEvent(QDropEvent *e);

    private slots:
        void on_editGroupBox_clicked(bool checked);
        void on_stopButton_clicked();
        void on_startButton_clicked();
        void timerUpdateGui();
        void taskStarting();
        void taskPrepared();
        void taskCopyDone();
        void taskfinished();
        void errorMessage(TaskThread *thread, int error);
        void copyError(TaskThread *thread, FileList list,int type);
        void threadMessage(QString msg,int *answer,int buttons);

        void expand();
        void contract();

        void sendDelayedFinished(){finishedEvent(this);}
        void on_copyRadioButton_clicked();
        void on_cutRadioButton_clicked();
        void on_speedSlider_inversed_sliderMoved(int position);
        void on_speedSlider_inversed_valueChanged(int position);
        void on_checkSumCheckBox_toggled(bool checked);
};

#endif // COPYINFOPANEL_H
