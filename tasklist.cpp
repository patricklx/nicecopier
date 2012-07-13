#include "tasklist.h"
#include "ui_tasklist.h"
#include <QScrollBar>
#include <QDebug>

class UserData : public QObjectUserData
{
        void* data;
    public:
        UserData(void* data){
            this->data = data;
        }
        void* getData(){
            return data;
        }
};

TaskList::TaskList(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TaskList)
{
    ui->setupUi(this);

    connect(ui->scrollArea->verticalScrollBar(),SIGNAL(valueChanged(int)),SLOT(scrollbar_changed(int)));
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea = ui->scrollAreaWidgetContents;

    size_x = 0;
    size_y = 0;
}

TaskList::~TaskList()
{
    delete ui;
}

QWidget *TaskList::getTask(int index)
{
    return ui->verticalLayout->itemAt(index)->widget();
}


void TaskList::scrollbar_changed(int val)
{
    int y = (val / size_y) * size_y;
    ui->scrollArea->verticalScrollBar()->setValue(y);
}

void TaskList::setMaxTask(int show)
{
    if(show<1)
    {
        max_show = 1;
    }else
        max_show = show;
    updateList();
}

void TaskList::updateList()
{
    int items = ui->verticalLayout->count();
    QSize size = QSize(0,0);


    if( items > max_show )
    {
        size.setHeight( size_y*max_show + 5 );
        size.setWidth(size_x + 20);
        ui->scrollArea->verticalScrollBar()->setFixedWidth(20);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }else
    {
        size.setHeight( size_y*items + 5 );
        size.setWidth(size_x);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    ui->scrollArea->setMaximumSize(size);
    ui->scrollArea->setMinimumSize(size);
    setMaximumSize(size);
    setMinimumSize(size);
    ui->scrollArea->verticalScrollBar()->setPageStep(size_y);
    ui->scrollArea->verticalScrollBar()->setSingleStep(size_y);
    resize(size);
}

void TaskList::removeTask(QWidget* task)
{
    int index = ui->verticalLayout->indexOf(task);
    if(index>0)
    {
        QLayoutItem* item = ui->verticalLayout->itemAt(index-1);
        QWidget* widget = item->widget();
        UserData *data = new UserData(widget);
        task->setUserData(0,data);
    }
    ui->verticalLayout->removeWidget(task);
    updateList();
}

void TaskList::insertTask(QWidget* task)
{
    UserData *data = (UserData*)task->userData(0);
    QWidget* widget = NULL;
    if(data)
        widget = (QWidget*) data->getData();

    int position = ui->verticalLayout->indexOf(widget)+1;
    qDebug()<<position;
    ui->verticalLayout->insertWidget(position,task);
    updateList();
}

void TaskList::setFirst(QWidget* item)
{
    if( ui->verticalLayout->indexOf(item) <= 0)
        return;
    ui->verticalLayout->removeWidget(item);
    ui->verticalLayout->insertWidget(0,item);
}

void TaskList::addTask( QWidget* p )
{
    ui->verticalLayout->addWidget(p);
    size_y = p->size().height()+4;//+size of layout
    size_x = p->size().width()+16;
    updateList();
}

int TaskList::count()
{
    return ui->verticalLayout->count();
}
