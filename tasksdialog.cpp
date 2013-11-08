#include "tasksdialog.h"
#include "ui_tasks_dialog_t.h"
#include "RecentTasks.h"
#include "TaskThread/taskthread.h"
#include "TaskWidget.h"
#include <QStringList>
#include <QBuffer>
#include <QWidget>
#include <QString>
#include "ncsettings.h"

TasksDialog::TasksDialog(QWidget *parent) :
    QDialog(parent,Qt::Dialog|Qt::WindowCloseButtonHint),
    ui(new Ui::TasksDialog)
{
    ui->setupUi(this);
    ui->scrollArea->hide();


    setPinnedTaskList();
    setRecentTaskList();

    show();
    this->adjustSize();
}


TasksDialog::~TasksDialog()
{
    delete ui;
}


void TasksDialog::adjustSize()
{
    update();
    ui->scrollAreaWidgetContents->adjustSize();
    int height = ui->scrollAreaWidgetContents->height();
    if(height>500)
        ui->scrollArea->setMinimumHeight(500);
    else
	ui->scrollArea->setMinimumHeight(height);

    QDialog::adjustSize();
}


void TasksDialog::setRecentTaskList()
{
    QStringList list = RecentTasks::getRecentTaskList();
    if(!list.isEmpty())
    {
        ui->lbNoRecent->hide();
        ui->scrollArea->show();
        for(int i=0;i<list.count();i++)
        {
            QString name = list[i];
            TaskWidget *task_widget = new TaskWidget(this,name);
            connect(task_widget,SIGNAL(toggledInfo()),SLOT(adjustSize()));
            connect(task_widget,SIGNAL(pinned(bool)),SLOT(togglePinnTask(bool)));
            connect(task_widget,SIGNAL(execute(QString)),SLOT(executeTask(QString)));
            ui->layout_recent->addWidget(task_widget);
        }
    }
}

void TasksDialog::setPinnedTaskList()
{
    QStringList list = RecentTasks::getPinnedTaskList();
    if(!list.isEmpty())
    {
        ui->lbNoRecent->hide();
        ui->scrollArea->show();
        for(int i=0;i<list.count();i++)
        {
            QString name = list[i];
            TaskWidget *task_widget = new TaskWidget(this,name);
            connect(task_widget,SIGNAL(toggledInfo()),SLOT(adjustSize()));
            connect(task_widget,SIGNAL(pinned(bool)),SLOT(togglePinnTask(bool)));
            connect(task_widget,SIGNAL(execute(QString)),SLOT(executeTask(QString)));

            task_widget->setPinned(true);
            ui->layout_pinned->addWidget(task_widget);

        }
    }
}


void TasksDialog::executeTask(QString task)
{
    choosen = task;
    RecentTasks::setTaskFirst(task);
    close();
}


void TasksDialog::togglePinnTask(bool pinn)
{
    QWidget *w = (QWidget*) sender();
    if(pinn)
    {
        ui->layout_recent->removeWidget(w);
        ui->layout_pinned->insertWidget(1,w);
        qDebug("TasksDialog: pinned");
    }

    if(!pinn)
    {
        ui->layout_pinned->removeWidget(w);
        ui->layout_recent->insertWidget(1,w);
        if( ui->layout_recent->count()-1 > NcSettings::getValue<int>(NcSettings::MAX_RECENT_TASKS) )
	    delete ui->layout_recent->takeAt(ui->layout_recent->count()-1);
        qDebug("TasksDialog: unPinned");
    }
}





