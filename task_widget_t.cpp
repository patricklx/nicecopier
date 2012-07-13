#include "TaskWidget_t.h"
#include "ui_task_widget_t.h"

#include <QBuffer>
#include "TaskThread/taskthread.h"
#include "RecentTasks.h"
#include <QInputDialog>
#include <QXmlStreamReader>

TaskWidget::TaskWidget(QWidget *parent,QString name) :
    QWidget(parent),
    ui(new Ui::task_widget_t)
{
    ui->setupUi(this);
    task = name;

    QString taskContent = RecentTasks::getTask(name);
    QByteArray array = taskContent.toUtf8();
    QBuffer buffer(&array);
    buffer.open(QIODevice::ReadOnly);
    TaskThread taskinfo(buffer,true);

    ui->lbFromPath->setText(taskinfo.getSourcePath());
    ui->lbToPath->setText(taskinfo.getDestinationPath());

    for(int a=0;a<taskinfo.getSourceList().count();a++)
    {
        QString filename = taskinfo.getSourceList()[a].afterLast("/");
        if(filename.isEmpty())
            filename = taskinfo.getSourceList()[a].beforeLast("/").afterLast("/")+"/";

        ui->fileslistWidget->addItem(filename);
    }

    QXmlStreamReader reader(array);
    reader.readNextStartElement();
    if ( reader.hasError() )
    {
        qWarning("Failed to load xml: %s\n\n %s",reader.errorString().toAscii().data(),array.data() );
        return;
    }

    QXmlStreamAttributes attr = reader.attributes();
    date = attr.value("DATE").toString();
    ui->groupBox_4->setTitle(name+"    "+date);
}

TaskWidget::~TaskWidget()
{
    delete ui;
}

void TaskWidget::setPinned(bool pinned)
{
    if(pinned)
    {
        ui->InfoWidget->hide();
        ui->btInfo->setChecked(false);
        ui->btPinn->setText("unPin");
    }
    else
        ui->btPinn->setText("Pin");
}

void TaskWidget::on_btExecute_clicked()
{
    emit execute(task);
}



void TaskWidget::on_btInfo_toggled(bool checked)
{
    if(checked)
        ui->InfoWidget->show();
    else
        ui->InfoWidget->hide();

    emit toggledInfo();
}

void TaskWidget::on_btPinn_clicked()
{
    if(ui->btPinn->text()=="unPin")
    {
        ui->btPinn->setText("Pin");
        RecentTasks::unPinnRecentTask(task);
        emit pinned(false);
        return;
    }

    if(ui->btPinn->text()=="Pin")
    {
        ui->btPinn->setText("unPin");
        RecentTasks::pinnRecentTask(task);
        emit pinned(true);
        return;
    }
}

void TaskWidget::on_btRename_clicked()
{
    QInputDialog input(this);
    input.setInputMode(QInputDialog::TextInput);
    input.setWindowTitle("Rename Task");
    input.setLabelText("Rename task to: ");

    input.exec();
    QString name = input.textValue();
    if( !RecentTasks::rename(task,name) )
    {
        QMessageBox::warning(QApplication::activeWindow(),"rename failed","failed to rename task to: "+name);
    }else
    {
        ui->groupBox_4->setTitle(name+"    "+date);
    }
}
