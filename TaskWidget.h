#ifndef TASK_WIDGET_T_H
#define TASK_WIDGET_T_H

#include <QWidget>

namespace Ui {
    class task_widget_t;
}

class TaskWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit TaskWidget(QWidget *parent,QString task);
        ~TaskWidget();

        void setPinned(bool pinned);

    signals:
        void execute(QString task);
        void pinned(bool pinn);
        void toggledInfo();

    private slots:
        void on_btExecute_clicked();
        void on_btInfo_toggled(bool checked);


        void on_btPinn_clicked();

        void on_btRename_clicked();


    private:
        Ui::task_widget_t *ui;
        QString task;
        QString date;
};

#endif // TASK_WIDGET_T_H
