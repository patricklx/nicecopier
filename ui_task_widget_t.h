/********************************************************************************
** Form generated from reading UI file 'task_widget_t.ui'
**
** Created: Sat 4. Aug 21:12:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TASK_WIDGET_T_H
#define UI_TASK_WIDGET_T_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qlabeleli.h"

QT_BEGIN_NAMESPACE

class Ui_task_widget_t
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btInfo;
    QPushButton *btPinn;
    QPushButton *btRename;
    QCommandLinkButton *btExecute;
    QWidget *InfoWidget;
    QVBoxLayout *layout_files;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbFrom;
    QLabelEli *lbFromPath;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lbTo;
    QLabelEli *lbToPath;
    QLabel *lbFiles;
    QListWidget *fileslistWidget;

    void setupUi(QWidget *task_widget_t)
    {
        if (task_widget_t->objectName().isEmpty())
            task_widget_t->setObjectName(QString::fromUtf8("task_widget_t"));
        task_widget_t->resize(430, 203);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(task_widget_t->sizePolicy().hasHeightForWidth());
        task_widget_t->setSizePolicy(sizePolicy);
        task_widget_t->setMaximumSize(QSize(430, 16777215));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        task_widget_t->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        task_widget_t->setFont(font);
        verticalLayout = new QVBoxLayout(task_widget_t);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_4 = new QGroupBox(task_widget_t);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(430, 0));
        groupBox_4->setMaximumSize(QSize(430, 16777215));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Verdana"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        groupBox_4->setFont(font1);
        verticalLayout_9 = new QVBoxLayout(groupBox_4);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, -1, -1, 5);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btInfo = new QPushButton(groupBox_4);
        btInfo->setObjectName(QString::fromUtf8("btInfo"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btInfo->sizePolicy().hasHeightForWidth());
        btInfo->setSizePolicy(sizePolicy1);
        btInfo->setMaximumSize(QSize(100, 16777215));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        btInfo->setFont(font2);
        btInfo->setFocusPolicy(Qt::NoFocus);
        btInfo->setCheckable(true);
        btInfo->setChecked(true);
        btInfo->setFlat(true);

        horizontalLayout_2->addWidget(btInfo);

        btPinn = new QPushButton(groupBox_4);
        btPinn->setObjectName(QString::fromUtf8("btPinn"));
        sizePolicy1.setHeightForWidth(btPinn->sizePolicy().hasHeightForWidth());
        btPinn->setSizePolicy(sizePolicy1);
        btPinn->setMaximumSize(QSize(100, 16777215));
        btPinn->setFont(font2);
        btPinn->setFocusPolicy(Qt::NoFocus);
        btPinn->setFlat(true);

        horizontalLayout_2->addWidget(btPinn);

        btRename = new QPushButton(groupBox_4);
        btRename->setObjectName(QString::fromUtf8("btRename"));
        sizePolicy1.setHeightForWidth(btRename->sizePolicy().hasHeightForWidth());
        btRename->setSizePolicy(sizePolicy1);
        btRename->setMaximumSize(QSize(100, 16777215));
        btRename->setFont(font2);
        btRename->setFocusPolicy(Qt::NoFocus);
        btRename->setFlat(true);

        horizontalLayout_2->addWidget(btRename);

        btExecute = new QCommandLinkButton(groupBox_4);
        btExecute->setObjectName(QString::fromUtf8("btExecute"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Segoe UI"));
        font3.setBold(false);
        font3.setWeight(50);
        btExecute->setFont(font3);
        btExecute->setFocusPolicy(Qt::NoFocus);
        btExecute->setAutoDefault(false);

        horizontalLayout_2->addWidget(btExecute);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_6->addLayout(verticalLayout_3);

        InfoWidget = new QWidget(groupBox_4);
        InfoWidget->setObjectName(QString::fromUtf8("InfoWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(InfoWidget->sizePolicy().hasHeightForWidth());
        InfoWidget->setSizePolicy(sizePolicy2);
        QFont font4;
        font4.setBold(false);
        font4.setWeight(50);
        InfoWidget->setFont(font4);
        layout_files = new QVBoxLayout(InfoWidget);
        layout_files->setSpacing(5);
        layout_files->setObjectName(QString::fromUtf8("layout_files"));
        layout_files->setContentsMargins(5, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lbFrom = new QLabel(InfoWidget);
        lbFrom->setObjectName(QString::fromUtf8("lbFrom"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lbFrom->sizePolicy().hasHeightForWidth());
        lbFrom->setSizePolicy(sizePolicy3);
        lbFrom->setMinimumSize(QSize(60, 0));
        lbFrom->setMaximumSize(QSize(60, 16777215));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Courier New"));
        font5.setBold(true);
        font5.setWeight(75);
        lbFrom->setFont(font5);

        horizontalLayout_4->addWidget(lbFrom);

        lbFromPath = new QLabelEli(InfoWidget);
        lbFromPath->setObjectName(QString::fromUtf8("lbFromPath"));
        sizePolicy2.setHeightForWidth(lbFromPath->sizePolicy().hasHeightForWidth());
        lbFromPath->setSizePolicy(sizePolicy2);
        lbFromPath->setMaximumSize(QSize(353, 16777215));
        lbFromPath->setFont(font4);

        horizontalLayout_4->addWidget(lbFromPath);


        layout_files->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lbTo = new QLabel(InfoWidget);
        lbTo->setObjectName(QString::fromUtf8("lbTo"));
        sizePolicy3.setHeightForWidth(lbTo->sizePolicy().hasHeightForWidth());
        lbTo->setSizePolicy(sizePolicy3);
        lbTo->setMinimumSize(QSize(60, 0));
        lbTo->setMaximumSize(QSize(60, 16777215));
        lbTo->setFont(font5);

        horizontalLayout_5->addWidget(lbTo);

        lbToPath = new QLabelEli(InfoWidget);
        lbToPath->setObjectName(QString::fromUtf8("lbToPath"));
        lbToPath->setMaximumSize(QSize(353, 16777215));
        lbToPath->setFont(font4);

        horizontalLayout_5->addWidget(lbToPath);


        layout_files->addLayout(horizontalLayout_5);

        lbFiles = new QLabel(InfoWidget);
        lbFiles->setObjectName(QString::fromUtf8("lbFiles"));
        lbFiles->setMaximumSize(QSize(421, 16777215));
        lbFiles->setFont(font5);

        layout_files->addWidget(lbFiles);

        fileslistWidget = new QListWidget(InfoWidget);
        fileslistWidget->setObjectName(QString::fromUtf8("fileslistWidget"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(fileslistWidget->sizePolicy().hasHeightForWidth());
        fileslistWidget->setSizePolicy(sizePolicy4);
        fileslistWidget->setMaximumSize(QSize(16777215, 100));

        layout_files->addWidget(fileslistWidget);


        verticalLayout_6->addWidget(InfoWidget);


        verticalLayout_9->addLayout(verticalLayout_6);


        verticalLayout->addWidget(groupBox_4);


        retranslateUi(task_widget_t);

        QMetaObject::connectSlotsByName(task_widget_t);
    } // setupUi

    void retranslateUi(QWidget *task_widget_t)
    {
        task_widget_t->setWindowTitle(QApplication::translate("task_widget_t", "Form", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("task_widget_t", "Task 1", 0, QApplication::UnicodeUTF8));
        btInfo->setText(QApplication::translate("task_widget_t", "Info", 0, QApplication::UnicodeUTF8));
        btPinn->setText(QApplication::translate("task_widget_t", "Pin", 0, QApplication::UnicodeUTF8));
        btRename->setText(QApplication::translate("task_widget_t", "Rename", 0, QApplication::UnicodeUTF8));
        btExecute->setText(QApplication::translate("task_widget_t", "Execute Task", 0, QApplication::UnicodeUTF8));
        lbFrom->setText(QApplication::translate("task_widget_t", "From: ", 0, QApplication::UnicodeUTF8));
        lbFromPath->setText(QApplication::translate("task_widget_t", "TextLabel", 0, QApplication::UnicodeUTF8));
        lbTo->setText(QApplication::translate("task_widget_t", "  To:", 0, QApplication::UnicodeUTF8));
        lbToPath->setText(QApplication::translate("task_widget_t", "TextLabel", 0, QApplication::UnicodeUTF8));
        lbFiles->setText(QApplication::translate("task_widget_t", "Files:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class task_widget_t: public Ui_task_widget_t {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASK_WIDGET_T_H
