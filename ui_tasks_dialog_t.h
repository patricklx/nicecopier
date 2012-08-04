/********************************************************************************
** Form generated from reading UI file 'tasks_dialog_t.ui'
**
** Created: Sat 4. Aug 21:12:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TASKS_DIALOG_T_H
#define UI_TASKS_DIALOG_T_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TasksDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *lbNoRecent;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *layout_pinned;
    QLabel *label_2;
    QVBoxLayout *layout_recent;
    QLabel *label;

    void setupUi(QDialog *TasksDialog)
    {
        if (TasksDialog->objectName().isEmpty())
            TasksDialog->setObjectName(QString::fromUtf8("TasksDialog"));
        TasksDialog->resize(470, 97);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TasksDialog->sizePolicy().hasHeightForWidth());
        TasksDialog->setSizePolicy(sizePolicy);
        TasksDialog->setMinimumSize(QSize(470, 0));
        TasksDialog->setMaximumSize(QSize(470, 500));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        TasksDialog->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        TasksDialog->setFont(font);
        verticalLayout_2 = new QVBoxLayout(TasksDialog);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 0, 0, 0);
        lbNoRecent = new QLabel(TasksDialog);
        lbNoRecent->setObjectName(QString::fromUtf8("lbNoRecent"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Courier New"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        lbNoRecent->setFont(font1);
        lbNoRecent->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lbNoRecent);

        scrollArea = new QScrollArea(TasksDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy1);
        scrollArea->setMinimumSize(QSize(465, 0));
        scrollArea->setMaximumSize(QSize(465, 500));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 448, 60));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setSpacing(10);
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        layout_pinned = new QVBoxLayout();
        layout_pinned->setObjectName(QString::fromUtf8("layout_pinned"));
        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);

        layout_pinned->addWidget(label_2);


        verticalLayout->addLayout(layout_pinned);

        layout_recent = new QVBoxLayout();
        layout_recent->setObjectName(QString::fromUtf8("layout_recent"));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        layout_recent->addWidget(label);


        verticalLayout->addLayout(layout_recent);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);


        retranslateUi(TasksDialog);

        QMetaObject::connectSlotsByName(TasksDialog);
    } // setupUi

    void retranslateUi(QDialog *TasksDialog)
    {
        TasksDialog->setWindowTitle(QApplication::translate("TasksDialog", "NiceCopier Recent Tasks", 0, QApplication::UnicodeUTF8));
        lbNoRecent->setText(QApplication::translate("TasksDialog", "No recent Tasks", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TasksDialog", "Pinned Tasks", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TasksDialog", "Recent Tasks", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TasksDialog: public Ui_TasksDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASKS_DIALOG_T_H
