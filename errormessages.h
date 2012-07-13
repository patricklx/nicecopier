#ifndef ERRORMESSAGES_H
#define ERRORMESSAGES_H




#include <QWidget>
#include <QDialog>
#include "TaskThread/taskthread.h"


namespace Ui {
    class ErrorMessages;
}

class DlgErrorMessages : public QDialog
{
        Q_OBJECT
    public:
        enum FileCopyErrors{
            ER_FILE_DEST_EXIST = 1,
            ER_GET_SIZE,
            ER_COPY,
            EDIT_MODE,
            FREE_SPACE_MODE
        };

        explicit DlgErrorMessages(QWidget *parent = 0);
        ~DlgErrorMessages();
        bool setErrors(TaskThread *thread,int type);
        bool setErrors(TaskThread *thread,QList<File*> &files,int type);
        void closeEvent(QCloseEvent *);

    private:
        Ui::ErrorMessages *ui;
        int m_error_type;
        TaskThread *m_thread;

    private slots:
        void on_BtRetry_clicked();
        void on_BtReplace_clicked();
        void on_BtRename_clicked();
        void on_BtIgnore_clicked();
        void on_BtCancel_clicked();
        void on_BtOk_clicked();
};

#endif // ERRORMESSAGES_H
