#include "shutdowntimerdialog.h"
#include "ui_shutdowntimerdialog.h"
#include <ncsettings.h>
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <reason.h>

ShutDownTimerDialog::ShutDownTimerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShutDownTimerDialog)
{
    ui->setupUi(this);
    time_passed=0;
    this->setWindowTitle("ShuttingDownt CountDown");

    this->move(NcSettings::screenCenter()-rect().bottomRight()/2);
}

ShutDownTimerDialog::~ShutDownTimerDialog()
{
    delete ui;
}

int ShutDownTimerDialog::exec()
{
    startTimer(1000);
    return QDialog::exec();
}

void ShutDownTimerDialog::on_pushButton_clicked()
{
    AbortSystemShutdown(NULL);
    close();
}


void ShutDownTimerDialog::timerEvent(QTimerEvent *e)
{
    time_passed++;
    ui->lb_time_remaining->setNum(60-time_passed);
    if(time_passed>60)
    {
        ExitWindowsEx(EWX_SHUTDOWN,SHTDN_REASON_MINOR_OTHER|SHTDN_REASON_FLAG_PLANNED);
        killTimer(e->timerId());
        close();
    }

}
