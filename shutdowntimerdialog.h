#ifndef SHUTDOWNTIMERDIALOG_H
#define SHUTDOWNTIMERDIALOG_H

#include <QWidget>
#include <QDialog>

namespace Ui {
    class ShutDownTimerDialog;
}

class ShutDownTimerDialog : public QDialog
{
    Q_OBJECT

public:
    ShutDownTimerDialog(QWidget *parent = 0);
    ~ShutDownTimerDialog();

    int time_passed;
    int exec();

private slots:
    void on_pushButton_clicked();
    void timerEvent(QTimerEvent *e);

private:
    Ui::ShutDownTimerDialog *ui;
};

#endif // SHUTDOWNTIMERDIALOG_H
