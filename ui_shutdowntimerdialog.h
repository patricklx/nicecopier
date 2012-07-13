/********************************************************************************
** Form generated from reading UI file 'shutdowntimerdialog.ui'
**
** Created: Fri 13. Jul 16:05:30 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHUTDOWNTIMERDIALOG_H
#define UI_SHUTDOWNTIMERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShutDownTimerDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLabel *lb_time_remaining;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ShutDownTimerDialog)
    {
        if (ShutDownTimerDialog->objectName().isEmpty())
            ShutDownTimerDialog->setObjectName(QString::fromUtf8("ShutDownTimerDialog"));
        ShutDownTimerDialog->resize(320, 102);
        verticalLayout = new QVBoxLayout(ShutDownTimerDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(ShutDownTimerDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        font.setPointSize(13);
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        label_2->setFont(font);

        horizontalLayout->addWidget(label_2);

        lb_time_remaining = new QLabel(ShutDownTimerDialog);
        lb_time_remaining->setObjectName(QString::fromUtf8("lb_time_remaining"));
        sizePolicy.setHeightForWidth(lb_time_remaining->sizePolicy().hasHeightForWidth());
        lb_time_remaining->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Verdana"));
        font1.setPointSize(13);
        font1.setBold(true);
        font1.setWeight(75);
        lb_time_remaining->setFont(font1);

        horizontalLayout->addWidget(lb_time_remaining);

        label = new QLabel(ShutDownTimerDialog);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setFont(font1);

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        pushButton = new QPushButton(ShutDownTimerDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMinimumSize(QSize(0, 50));
        QFont font2;
        font2.setPointSize(15);
        font2.setBold(false);
        font2.setWeight(50);
        pushButton->setFont(font2);

        verticalLayout->addWidget(pushButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ShutDownTimerDialog);

        QMetaObject::connectSlotsByName(ShutDownTimerDialog);
    } // setupUi

    void retranslateUi(QWidget *ShutDownTimerDialog)
    {
        ShutDownTimerDialog->setWindowTitle(QApplication::translate("ShutDownTimerDialog", "ShuttingDownt CountDown", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ShutDownTimerDialog", "Shutting down System in:", 0, QApplication::UnicodeUTF8));
        lb_time_remaining->setText(QApplication::translate("ShutDownTimerDialog", "60", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ShutDownTimerDialog", "s", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ShutDownTimerDialog", "CANCEL", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ShutDownTimerDialog: public Ui_ShutDownTimerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHUTDOWNTIMERDIALOG_H
