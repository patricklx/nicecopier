/********************************************************************************
** Form generated from reading UI file 'ncsettings.ui'
**
** Created: Sat 4. Aug 21:12:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NCSETTINGS_H
#define UI_NCSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ncsettings
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *ck_start_wsystem;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *cbadmin;
    QCheckBox *ckUpdates;
    QCheckBox *ck_nc_onTop;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *cbappearance;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QSpinBox *sb_show_tasks;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QSpinBox *sb_max_recentTasks;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *gb_position;
    QFormLayout *formLayout;
    QCheckBox *cbTopLeft;
    QCheckBox *cbTopRight;
    QCheckBox *cbBottomLeft;
    QCheckBox *cbBottomRight;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *ck_start;
    QHBoxLayout *horizontalLayout_6;
    QSpinBox *timeUntilShow;
    QLabel *label_5;
    QCheckBox *ck_cancel_confirm;
    QGroupBox *ck_delete_dest;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *cb_confirm_delete;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QComboBox *cb_default_handle;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *sb_sec_del;
    QLabel *label;
    QCheckBox *defaultIcons;
    QCheckBox *ckCheckSum;
    QSpacerItem *verticalSpacer;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *usePlastiqueStyle;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *progressBarColor;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QComboBox *styleComboBox;
    QTextBrowser *textBrowser;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btCancel;
    QPushButton *btApply;
    QPushButton *btOk;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *ncsettings)
    {
        if (ncsettings->objectName().isEmpty())
            ncsettings->setObjectName(QString::fromUtf8("ncsettings"));
        ncsettings->resize(338, 366);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        ncsettings->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Verdana"));
        font.setPointSize(10);
        ncsettings->setFont(font);
        verticalLayout = new QVBoxLayout(ncsettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(ncsettings);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ck_start_wsystem = new QGroupBox(tab);
        ck_start_wsystem->setObjectName(QString::fromUtf8("ck_start_wsystem"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ck_start_wsystem->sizePolicy().hasHeightForWidth());
        ck_start_wsystem->setSizePolicy(sizePolicy);
        ck_start_wsystem->setFlat(false);
        ck_start_wsystem->setCheckable(true);
        ck_start_wsystem->setChecked(false);
        verticalLayout_5 = new QVBoxLayout(ck_start_wsystem);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(20, 0, 0, 0);
        cbadmin = new QCheckBox(ck_start_wsystem);
        cbadmin->setObjectName(QString::fromUtf8("cbadmin"));

        verticalLayout_5->addWidget(cbadmin);


        verticalLayout_2->addWidget(ck_start_wsystem);

        ckUpdates = new QCheckBox(tab);
        ckUpdates->setObjectName(QString::fromUtf8("ckUpdates"));

        verticalLayout_2->addWidget(ckUpdates);

        ck_nc_onTop = new QCheckBox(tab);
        ck_nc_onTop->setObjectName(QString::fromUtf8("ck_nc_onTop"));

        verticalLayout_2->addWidget(ck_nc_onTop);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(label_2);

        cbappearance = new QComboBox(tab);
        cbappearance->setObjectName(QString::fromUtf8("cbappearance"));
        cbappearance->setEditable(false);
        cbappearance->setInsertPolicy(QComboBox::NoInsert);
        cbappearance->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        cbappearance->setDuplicatesEnabled(false);
        cbappearance->setFrame(true);

        horizontalLayout_3->addWidget(cbappearance);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(label_3);

        sb_show_tasks = new QSpinBox(tab);
        sb_show_tasks->setObjectName(QString::fromUtf8("sb_show_tasks"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sb_show_tasks->sizePolicy().hasHeightForWidth());
        sb_show_tasks->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(sb_show_tasks);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(label_6);

        sb_max_recentTasks = new QSpinBox(tab);
        sb_max_recentTasks->setObjectName(QString::fromUtf8("sb_max_recentTasks"));
        sizePolicy2.setHeightForWidth(sb_max_recentTasks->sizePolicy().hasHeightForWidth());
        sb_max_recentTasks->setSizePolicy(sizePolicy2);
        sb_max_recentTasks->setWrapping(false);

        horizontalLayout_7->addWidget(sb_max_recentTasks);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_7);

        gb_position = new QGroupBox(tab);
        gb_position->setObjectName(QString::fromUtf8("gb_position"));
        gb_position->setCheckable(false);
        formLayout = new QFormLayout(gb_position);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        cbTopLeft = new QCheckBox(gb_position);
        cbTopLeft->setObjectName(QString::fromUtf8("cbTopLeft"));
        cbTopLeft->setChecked(false);
        cbTopLeft->setAutoRepeat(false);
        cbTopLeft->setAutoExclusive(true);
        cbTopLeft->setTristate(false);

        formLayout->setWidget(0, QFormLayout::LabelRole, cbTopLeft);

        cbTopRight = new QCheckBox(gb_position);
        cbTopRight->setObjectName(QString::fromUtf8("cbTopRight"));
        cbTopRight->setAutoExclusive(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, cbTopRight);

        cbBottomLeft = new QCheckBox(gb_position);
        cbBottomLeft->setObjectName(QString::fromUtf8("cbBottomLeft"));
        cbBottomLeft->setAutoExclusive(true);

        formLayout->setWidget(1, QFormLayout::LabelRole, cbBottomLeft);

        cbBottomRight = new QCheckBox(gb_position);
        cbBottomRight->setObjectName(QString::fromUtf8("cbBottomRight"));
        cbBottomRight->setAutoExclusive(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, cbBottomRight);


        verticalLayout_2->addWidget(gb_position);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        ck_start = new QCheckBox(tab_2);
        ck_start->setObjectName(QString::fromUtf8("ck_start"));

        verticalLayout_3->addWidget(ck_start);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        timeUntilShow = new QSpinBox(tab_2);
        timeUntilShow->setObjectName(QString::fromUtf8("timeUntilShow"));
        sizePolicy2.setHeightForWidth(timeUntilShow->sizePolicy().hasHeightForWidth());
        timeUntilShow->setSizePolicy(sizePolicy2);
        timeUntilShow->setWrapping(false);
        timeUntilShow->setFrame(true);
        timeUntilShow->setMaximum(5000);
        timeUntilShow->setValue(2000);

        horizontalLayout_6->addWidget(timeUntilShow);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);


        verticalLayout_3->addLayout(horizontalLayout_6);

        ck_cancel_confirm = new QCheckBox(tab_2);
        ck_cancel_confirm->setObjectName(QString::fromUtf8("ck_cancel_confirm"));

        verticalLayout_3->addWidget(ck_cancel_confirm);

        ck_delete_dest = new QGroupBox(tab_2);
        ck_delete_dest->setObjectName(QString::fromUtf8("ck_delete_dest"));
        ck_delete_dest->setCheckable(true);
        ck_delete_dest->setChecked(false);
        verticalLayout_4 = new QVBoxLayout(ck_delete_dest);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        cb_confirm_delete = new QCheckBox(ck_delete_dest);
        cb_confirm_delete->setObjectName(QString::fromUtf8("cb_confirm_delete"));

        verticalLayout_4->addWidget(cb_confirm_delete);


        verticalLayout_3->addWidget(ck_delete_dest);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(label_4);

        cb_default_handle = new QComboBox(tab_2);
        cb_default_handle->setObjectName(QString::fromUtf8("cb_default_handle"));
        sizePolicy2.setHeightForWidth(cb_default_handle->sizePolicy().hasHeightForWidth());
        cb_default_handle->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(cb_default_handle);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        sb_sec_del = new QSpinBox(tab_2);
        sb_sec_del->setObjectName(QString::fromUtf8("sb_sec_del"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(sb_sec_del->sizePolicy().hasHeightForWidth());
        sb_sec_del->setSizePolicy(sizePolicy3);
        sb_sec_del->setValue(5);

        horizontalLayout_2->addWidget(sb_sec_del);

        label = new QLabel(tab_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);


        verticalLayout_3->addLayout(horizontalLayout_2);

        defaultIcons = new QCheckBox(tab_2);
        defaultIcons->setObjectName(QString::fromUtf8("defaultIcons"));

        verticalLayout_3->addWidget(defaultIcons);

        ckCheckSum = new QCheckBox(tab_2);
        ckCheckSum->setObjectName(QString::fromUtf8("ckCheckSum"));

        verticalLayout_3->addWidget(ckCheckSum);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        usePlastiqueStyle = new QGroupBox(tab_3);
        usePlastiqueStyle->setObjectName(QString::fromUtf8("usePlastiqueStyle"));
        usePlastiqueStyle->setCheckable(true);
        usePlastiqueStyle->setChecked(false);
        horizontalLayout_8 = new QHBoxLayout(usePlastiqueStyle);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        progressBarColor = new QPushButton(usePlastiqueStyle);
        progressBarColor->setObjectName(QString::fromUtf8("progressBarColor"));

        horizontalLayout_8->addWidget(progressBarColor);

        progressBar = new QProgressBar(usePlastiqueStyle);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(60);
        progressBar->setTextVisible(false);

        horizontalLayout_8->addWidget(progressBar);


        verticalLayout_6->addWidget(usePlastiqueStyle);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_7 = new QLabel(tab_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_10->addWidget(label_7);

        styleComboBox = new QComboBox(tab_3);
        styleComboBox->setObjectName(QString::fromUtf8("styleComboBox"));

        horizontalLayout_10->addWidget(styleComboBox);


        verticalLayout_6->addLayout(horizontalLayout_10);

        textBrowser = new QTextBrowser(tab_3);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
        textBrowser->setOpenExternalLinks(true);

        verticalLayout_6->addWidget(textBrowser);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);

        tabWidget->addTab(tab_3, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btCancel = new QPushButton(ncsettings);
        btCancel->setObjectName(QString::fromUtf8("btCancel"));

        horizontalLayout->addWidget(btCancel);

        btApply = new QPushButton(ncsettings);
        btApply->setObjectName(QString::fromUtf8("btApply"));

        horizontalLayout->addWidget(btApply);

        btOk = new QPushButton(ncsettings);
        btOk->setObjectName(QString::fromUtf8("btOk"));

        horizontalLayout->addWidget(btOk);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ncsettings);

        tabWidget->setCurrentIndex(0);
        cbappearance->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ncsettings);
    } // setupUi

    void retranslateUi(QWidget *ncsettings)
    {
        ncsettings->setWindowTitle(QApplication::translate("ncsettings", "Form", 0, QApplication::UnicodeUTF8));
        ck_start_wsystem->setTitle(QApplication::translate("ncsettings", "Start with system", 0, QApplication::UnicodeUTF8));
        cbadmin->setText(QApplication::translate("ncsettings", "Run as admin", 0, QApplication::UnicodeUTF8));
        ckUpdates->setText(QApplication::translate("ncsettings", "Check for Updates", 0, QApplication::UnicodeUTF8));
        ck_nc_onTop->setText(QApplication::translate("ncsettings", "always on top", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ncsettings", "Appearance", 0, QApplication::UnicodeUTF8));
        cbappearance->clear();
        cbappearance->insertItems(0, QStringList()
         << QApplication::translate("ncsettings", "Tool window", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ncsettings", "Dialog window", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ncsettings", "Normal window", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("ncsettings", "Tasks to show", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ncsettings", "Maximum Recent Tasks:", 0, QApplication::UnicodeUTF8));
        gb_position->setTitle(QApplication::translate("ncsettings", "Position", 0, QApplication::UnicodeUTF8));
        cbTopLeft->setText(QApplication::translate("ncsettings", "Top left", 0, QApplication::UnicodeUTF8));
        cbTopRight->setText(QApplication::translate("ncsettings", "Top right", 0, QApplication::UnicodeUTF8));
        cbBottomLeft->setText(QApplication::translate("ncsettings", "Bottom left", 0, QApplication::UnicodeUTF8));
        cbBottomRight->setText(QApplication::translate("ncsettings", "Bottom right", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ncsettings", "General", 0, QApplication::UnicodeUTF8));
        ck_start->setText(QApplication::translate("ncsettings", "Start copy immediately", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ncsettings", "milliseconds until task shows up", 0, QApplication::UnicodeUTF8));
        ck_cancel_confirm->setText(QApplication::translate("ncsettings", "Confirm if i really want to cancel", 0, QApplication::UnicodeUTF8));
        ck_delete_dest->setTitle(QApplication::translate("ncsettings", "Delete copied files when canceled", 0, QApplication::UnicodeUTF8));
        cb_confirm_delete->setText(QApplication::translate("ncsettings", "Confirm delete of copied files", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ncsettings", "Default Handle if file exists:", 0, QApplication::UnicodeUTF8));
        cb_default_handle->clear();
        cb_default_handle->insertItems(0, QStringList()
         << QApplication::translate("ncsettings", "Replace", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ncsettings", "Rename", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ncsettings", "Ignore", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("ncsettings", "sec until deletion of source", 0, QApplication::UnicodeUTF8));
        defaultIcons->setText(QApplication::translate("ncsettings", "show default Icons ( is faster )", 0, QApplication::UnicodeUTF8));
        ckCheckSum->setText(QApplication::translate("ncsettings", "verify file Checksum", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ncsettings", "Copy settings", 0, QApplication::UnicodeUTF8));
        usePlastiqueStyle->setTitle(QApplication::translate("ncsettings", "Use Plastique Style", 0, QApplication::UnicodeUTF8));
        progressBarColor->setText(QApplication::translate("ncsettings", "ProgressBar Color", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ncsettings", "Select a style:", 0, QApplication::UnicodeUTF8));
        styleComboBox->clear();
        styleComboBox->insertItems(0, QStringList()
         << QApplication::translate("ncsettings", "None", 0, QApplication::UnicodeUTF8)
        );
        textBrowser->setHtml(QApplication::translate("ncsettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Verdana'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">If you also want to create you own NiceCopier stylesheet visit</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://nicecopier.sourceforge.net/\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">http://nicecopier.sourceforge.net/styling.html</span></a><span style=\" font-size:8pt;\"> </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-ind"
                        "ent:0px;\"><span style=\" font-size:8pt;\">and</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://doc.qt.nokia.com/4.7-snapshot/stylesheet-reference.html\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">http://doc.qt.nokia.com/4.7-snapshot/stylesheet-reference.html</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("ncsettings", "Style", 0, QApplication::UnicodeUTF8));
        btCancel->setText(QApplication::translate("ncsettings", "Cancel", 0, QApplication::UnicodeUTF8));
        btApply->setText(QApplication::translate("ncsettings", "Apply", 0, QApplication::UnicodeUTF8));
        btOk->setText(QApplication::translate("ncsettings", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ncsettings: public Ui_ncsettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NCSETTINGS_H
