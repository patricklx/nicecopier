#ifndef TASKS_DIALOG_T_H
#define TASKS_DIALOG_T_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <ui_tasks_dialog_t.h>

namespace Ui {
    class TasksDialog;
}

class TasksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TasksDialog(QWidget *parent = 0);
    ~TasksDialog();
    QString choosen;

    QString returnValue();
    void setRecentTaskList();
    void setPinnedTaskList();

private slots:
    void adjustSize();
    void togglePinnTask(bool pinn);
    void executeTask(QString task);

private:
    Ui::TasksDialog *ui;

};

#endif // TASKS_DIALOG_T_H
