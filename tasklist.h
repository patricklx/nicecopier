#ifndef TASKLIST_H
#define TASKLIST_H

#include <QWidget>

namespace Ui {
    class TaskList;
}

class TaskList : public QWidget
{
        Q_OBJECT


    public:
        explicit TaskList(QWidget *parent = 0);
        ~TaskList();

        void addTask(QWidget *item);
        void setMaxTask(int num);
        void removeTask(QWidget *task);
        void setFirst(QWidget *item);
        void insertTask(QWidget* task);
        int count();
        void updateList();
        QWidget *getTask(int index);

        QWidget* getScrollArea(){
            return scrollArea;
        }

    private:
        int max_show;
        QWidget *scrollArea;
        Ui::TaskList *ui;
        int size_y,size_x;

    private slots:

        void scrollbar_changed(int val);
};

#endif // TASKLIST_H
