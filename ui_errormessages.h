/********************************************************************************
** Form generated from reading UI file 'errormessages.ui'
**
** Created: Sat 4. Aug 21:12:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ERRORMESSAGES_H
#define UI_ERRORMESSAGES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "filelistwidget.h"
#include "qlabeleli.h"

QT_BEGIN_NAMESPACE

class Ui_ErrorMessages
{
public:
    QHBoxLayout *horizontalLayout_31;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_34;
    QLabel *labelSource;
    QLabelEli *lbSource;
    QHBoxLayout *horizontalLayout_35;
    QLabel *labelDestination;
    QLabelEli *lbDestination;
    QHBoxLayout *horizontalLayout_32;
    FileListWidget *treeWidget;
    QHBoxLayout *horizontalLayout_36;
    QCommandLinkButton *BtIgnore;
    QCommandLinkButton *BtRetry;
    QCommandLinkButton *BtRename;
    QCommandLinkButton *BtReplace;
    QHBoxLayout *horizontalLayout_33;
    QSpacerItem *horizontalSpacer_13;
    QCommandLinkButton *BtCancel;
    QCommandLinkButton *BtOk;
    QSpacerItem *horizontalSpacer_14;

    void setupUi(QWidget *ErrorMessages)
    {
        if (ErrorMessages->objectName().isEmpty())
            ErrorMessages->setObjectName(QString::fromUtf8("ErrorMessages"));
        ErrorMessages->resize(886, 300);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        ErrorMessages->setPalette(palette);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        ErrorMessages->setFont(font);
        horizontalLayout_31 = new QHBoxLayout(ErrorMessages);
        horizontalLayout_31->setObjectName(QString::fromUtf8("horizontalLayout_31"));
        frame = new QFrame(ErrorMessages);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Verdana"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setUnderline(true);
        font1.setWeight(75);
        font1.setStrikeOut(false);
        font1.setKerning(true);
        font1.setStyleStrategy(QFont::PreferDefault);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        horizontalLayout_34 = new QHBoxLayout();
        horizontalLayout_34->setObjectName(QString::fromUtf8("horizontalLayout_34"));
        labelSource = new QLabel(frame);
        labelSource->setObjectName(QString::fromUtf8("labelSource"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelSource->sizePolicy().hasHeightForWidth());
        labelSource->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        labelSource->setFont(font2);

        horizontalLayout_34->addWidget(labelSource);

        lbSource = new QLabelEli(frame);
        lbSource->setObjectName(QString::fromUtf8("lbSource"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbSource->sizePolicy().hasHeightForWidth());
        lbSource->setSizePolicy(sizePolicy1);
        QFont font3;
        font3.setPointSize(10);
        lbSource->setFont(font3);
        lbSource->setScaledContents(false);

        horizontalLayout_34->addWidget(lbSource);


        verticalLayout->addLayout(horizontalLayout_34);

        horizontalLayout_35 = new QHBoxLayout();
        horizontalLayout_35->setObjectName(QString::fromUtf8("horizontalLayout_35"));
        labelDestination = new QLabel(frame);
        labelDestination->setObjectName(QString::fromUtf8("labelDestination"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelDestination->sizePolicy().hasHeightForWidth());
        labelDestination->setSizePolicy(sizePolicy2);
        labelDestination->setFont(font2);

        horizontalLayout_35->addWidget(labelDestination);

        lbDestination = new QLabelEli(frame);
        lbDestination->setObjectName(QString::fromUtf8("lbDestination"));
        sizePolicy1.setHeightForWidth(lbDestination->sizePolicy().hasHeightForWidth());
        lbDestination->setSizePolicy(sizePolicy1);
        lbDestination->setFont(font3);
        lbDestination->setScaledContents(false);

        horizontalLayout_35->addWidget(lbDestination);


        verticalLayout->addLayout(horizontalLayout_35);

        horizontalLayout_32 = new QHBoxLayout();
        horizontalLayout_32->setObjectName(QString::fromUtf8("horizontalLayout_32"));
        treeWidget = new FileListWidget(frame);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy3);
        treeWidget->setFocusPolicy(Qt::StrongFocus);

        horizontalLayout_32->addWidget(treeWidget);


        verticalLayout->addLayout(horizontalLayout_32);

        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(0);
        horizontalLayout_36->setObjectName(QString::fromUtf8("horizontalLayout_36"));
        BtIgnore = new QCommandLinkButton(frame);
        BtIgnore->setObjectName(QString::fromUtf8("BtIgnore"));
        sizePolicy.setHeightForWidth(BtIgnore->sizePolicy().hasHeightForWidth());
        BtIgnore->setSizePolicy(sizePolicy);
        BtIgnore->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/ignore.ico"), QSize(), QIcon::Normal, QIcon::Off);
        BtIgnore->setIcon(icon);
        BtIgnore->setIconSize(QSize(32, 32));

        horizontalLayout_36->addWidget(BtIgnore);

        BtRetry = new QCommandLinkButton(frame);
        BtRetry->setObjectName(QString::fromUtf8("BtRetry"));
        sizePolicy.setHeightForWidth(BtRetry->sizePolicy().hasHeightForWidth());
        BtRetry->setSizePolicy(sizePolicy);
        BtRetry->setMinimumSize(QSize(0, 50));
        BtRetry->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/retry.ico"), QSize(), QIcon::Normal, QIcon::Off);
        BtRetry->setIcon(icon1);
        BtRetry->setIconSize(QSize(32, 32));

        horizontalLayout_36->addWidget(BtRetry);

        BtRename = new QCommandLinkButton(frame);
        BtRename->setObjectName(QString::fromUtf8("BtRename"));
        sizePolicy.setHeightForWidth(BtRename->sizePolicy().hasHeightForWidth());
        BtRename->setSizePolicy(sizePolicy);
        BtRename->setMinimumSize(QSize(0, 50));
        BtRename->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/rename.ico"), QSize(), QIcon::Normal, QIcon::Off);
        BtRename->setIcon(icon2);
        BtRename->setIconSize(QSize(32, 32));

        horizontalLayout_36->addWidget(BtRename);

        BtReplace = new QCommandLinkButton(frame);
        BtReplace->setObjectName(QString::fromUtf8("BtReplace"));
        sizePolicy2.setHeightForWidth(BtReplace->sizePolicy().hasHeightForWidth());
        BtReplace->setSizePolicy(sizePolicy2);
        BtReplace->setMinimumSize(QSize(0, 50));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Segoe UI"));
        BtReplace->setFont(font4);
        BtReplace->setFocusPolicy(Qt::NoFocus);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/copy.ico"), QSize(), QIcon::Normal, QIcon::Off);
        BtReplace->setIcon(icon3);
        BtReplace->setIconSize(QSize(32, 32));

        horizontalLayout_36->addWidget(BtReplace);


        verticalLayout->addLayout(horizontalLayout_36);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setSpacing(0);
        horizontalLayout_33->setObjectName(QString::fromUtf8("horizontalLayout_33"));
        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_33->addItem(horizontalSpacer_13);

        BtCancel = new QCommandLinkButton(frame);
        BtCancel->setObjectName(QString::fromUtf8("BtCancel"));
        BtCancel->setMinimumSize(QSize(50, 0));
        BtCancel->setMaximumSize(QSize(150, 16777215));
        BtCancel->setFocusPolicy(Qt::NoFocus);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/cancel.ico"), QSize(), QIcon::Normal, QIcon::Off);
        BtCancel->setIcon(icon4);
        BtCancel->setIconSize(QSize(32, 32));
        BtCancel->setAutoExclusive(false);
        BtCancel->setAutoDefault(false);
        BtCancel->setDefault(false);

        horizontalLayout_33->addWidget(BtCancel);

        BtOk = new QCommandLinkButton(frame);
        BtOk->setObjectName(QString::fromUtf8("BtOk"));
        BtOk->setEnabled(true);
        sizePolicy.setHeightForWidth(BtOk->sizePolicy().hasHeightForWidth());
        BtOk->setSizePolicy(sizePolicy);
        BtOk->setMinimumSize(QSize(0, 0));
        BtOk->setMaximumSize(QSize(150, 16777215));
        BtOk->setFocusPolicy(Qt::NoFocus);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/check.ico"), QSize(), QIcon::Normal, QIcon::Off);
        BtOk->setIcon(icon5);
        BtOk->setIconSize(QSize(32, 32));

        horizontalLayout_33->addWidget(BtOk);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_33->addItem(horizontalSpacer_14);


        verticalLayout->addLayout(horizontalLayout_33);


        horizontalLayout_31->addWidget(frame);


        retranslateUi(ErrorMessages);

        QMetaObject::connectSlotsByName(ErrorMessages);
    } // setupUi

    void retranslateUi(QWidget *ErrorMessages)
    {
        ErrorMessages->setWindowTitle(QApplication::translate("ErrorMessages", "NiceCopier message", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ErrorMessages", "Files already exist in destination!", 0, QApplication::UnicodeUTF8));
        labelSource->setText(QApplication::translate("ErrorMessages", "Source:", 0, QApplication::UnicodeUTF8));
        lbSource->setText(QApplication::translate("ErrorMessages", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelDestination->setText(QApplication::translate("ErrorMessages", "Destination :", 0, QApplication::UnicodeUTF8));
        lbDestination->setText(QApplication::translate("ErrorMessages", "TextLabel", 0, QApplication::UnicodeUTF8));
        BtIgnore->setText(QApplication::translate("ErrorMessages", "Ignore", 0, QApplication::UnicodeUTF8));
        BtRetry->setText(QApplication::translate("ErrorMessages", "Retry", 0, QApplication::UnicodeUTF8));
        BtRename->setText(QApplication::translate("ErrorMessages", "Rename", 0, QApplication::UnicodeUTF8));
        BtReplace->setText(QApplication::translate("ErrorMessages", "Replace", 0, QApplication::UnicodeUTF8));
        BtCancel->setText(QApplication::translate("ErrorMessages", "CANCEL", 0, QApplication::UnicodeUTF8));
        BtOk->setText(QApplication::translate("ErrorMessages", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ErrorMessages: public Ui_ErrorMessages {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERRORMESSAGES_H
