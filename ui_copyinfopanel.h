/********************************************************************************
** Form generated from reading UI file 'copyinfopanel.ui'
**
** Created: Sat 4. Aug 21:12:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COPYINFOPANEL_H
#define UI_COPYINFOPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "filelistwidget.h"
#include "myprogressbar.h"
#include "qlabeleli.h"

QT_BEGIN_NAMESPACE

class Ui_copyInfoPanel
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout;
    QLabel *fromLabel;
    QLabelEli *fromPathLabel;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *targetLabel;
    QLabelEli *targetPathLabel;
    QHBoxLayout *horizontalLayout_4;
    QLabel *fileLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabelEli *currentFileLabel;
    QLabel *fileSizeLabel;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *stopButton;
    QPushButton *startButton;
    myprogressbar *progressBar;
    QHBoxLayout *horizontalLayout_5;
    QLabel *sizeLabel;
    QLabel *ofLabel;
    QLabel *totalSizeLabel;
    QLabel *speedLabel;
    QLabel *fileCountLabel;
    QSpacerItem *horizontalSpacer;
    QWidget *infoWidget;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_7;
    QRadioButton *cutRadioButton;
    QRadioButton *copyRadioButton;
    QCheckBox *checkSumCheckBox;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label;
    QSlider *speedSlider_inversed;
    QLabel *speedLevelLabel;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *editCheckBox;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *editGroupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_5;
    QPushButton *addFileButton;
    QPushButton *addFolderButton;
    QSpacerItem *verticalSpacer;
    FileListWidget *copyFileTree;

    void setupUi(QFrame *copyInfoPanel)
    {
        if (copyInfoPanel->objectName().isEmpty())
            copyInfoPanel->setObjectName(QString::fromUtf8("copyInfoPanel"));
        copyInfoPanel->resize(425, 364);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(copyInfoPanel->sizePolicy().hasHeightForWidth());
        copyInfoPanel->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        copyInfoPanel->setPalette(palette);
        verticalLayout = new QVBoxLayout(copyInfoPanel);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        groupBox = new QGroupBox(copyInfoPanel);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        font.setStyleStrategy(QFont::PreferDefault);
        groupBox->setFont(font);
        groupBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        groupBox->setFlat(false);
        groupBox->setChecked(false);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 0, 3, 3);
        frame = new QFrame(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(frame);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        fromLabel = new QLabel(frame);
        fromLabel->setObjectName(QString::fromUtf8("fromLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(fromLabel->sizePolicy().hasHeightForWidth());
        fromLabel->setSizePolicy(sizePolicy2);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Courier New"));
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        fromLabel->setFont(font1);

        horizontalLayout->addWidget(fromLabel);

        fromPathLabel = new QLabelEli(frame);
        fromPathLabel->setObjectName(QString::fromUtf8("fromPathLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(fromPathLabel->sizePolicy().hasHeightForWidth());
        fromPathLabel->setSizePolicy(sizePolicy3);
        QFont font2;
        font2.setBold(false);
        font2.setWeight(50);
        fromPathLabel->setFont(font2);

        horizontalLayout->addWidget(fromPathLabel);

        horizontalSpacer_3 = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout_7->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        targetLabel = new QLabel(frame);
        targetLabel->setObjectName(QString::fromUtf8("targetLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(targetLabel->sizePolicy().hasHeightForWidth());
        targetLabel->setSizePolicy(sizePolicy4);
        targetLabel->setFont(font1);
        targetLabel->setLayoutDirection(Qt::LeftToRight);
        targetLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(targetLabel);

        targetPathLabel = new QLabelEli(frame);
        targetPathLabel->setObjectName(QString::fromUtf8("targetPathLabel"));
        sizePolicy3.setHeightForWidth(targetPathLabel->sizePolicy().hasHeightForWidth());
        targetPathLabel->setSizePolicy(sizePolicy3);
        QFont font3;
        font3.setBold(false);
        font3.setItalic(false);
        font3.setUnderline(false);
        font3.setWeight(50);
        font3.setStrikeOut(false);
        font3.setKerning(true);
        font3.setStyleStrategy(QFont::PreferDefault);
        targetPathLabel->setFont(font3);
        targetPathLabel->setAcceptDrops(false);
        targetPathLabel->setAutoFillBackground(false);
        targetPathLabel->setFrameShape(QFrame::NoFrame);

        horizontalLayout_3->addWidget(targetPathLabel);


        verticalLayout_7->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        fileLabel = new QLabel(frame);
        fileLabel->setObjectName(QString::fromUtf8("fileLabel"));
        QSizePolicy sizePolicy5(QSizePolicy::Maximum, QSizePolicy::Minimum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(fileLabel->sizePolicy().hasHeightForWidth());
        fileLabel->setSizePolicy(sizePolicy5);
        fileLabel->setFont(font1);

        horizontalLayout_4->addWidget(fileLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        currentFileLabel = new QLabelEli(frame);
        currentFileLabel->setObjectName(QString::fromUtf8("currentFileLabel"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(currentFileLabel->sizePolicy().hasHeightForWidth());
        currentFileLabel->setSizePolicy(sizePolicy6);
        QFont font4;
        font4.setBold(true);
        font4.setItalic(true);
        font4.setWeight(75);
        currentFileLabel->setFont(font4);
        currentFileLabel->setLayoutDirection(Qt::LeftToRight);
        currentFileLabel->setFrameShape(QFrame::NoFrame);
        currentFileLabel->setIndent(0);

        horizontalLayout_2->addWidget(currentFileLabel);

        fileSizeLabel = new QLabel(frame);
        fileSizeLabel->setObjectName(QString::fromUtf8("fileSizeLabel"));
        QSizePolicy sizePolicy7(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(fileSizeLabel->sizePolicy().hasHeightForWidth());
        fileSizeLabel->setSizePolicy(sizePolicy7);
        QFont font5;
        font5.setPointSize(9);
        font5.setBold(true);
        font5.setWeight(75);
        fileSizeLabel->setFont(font5);
        fileSizeLabel->setFrameShape(QFrame::NoFrame);
        fileSizeLabel->setIndent(10);

        horizontalLayout_2->addWidget(fileSizeLabel);


        horizontalLayout_4->addLayout(horizontalLayout_2);


        verticalLayout_7->addLayout(horizontalLayout_4);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        stopButton = new QPushButton(frame);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        QSizePolicy sizePolicy8(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(stopButton->sizePolicy().hasHeightForWidth());
        stopButton->setSizePolicy(sizePolicy8);
        stopButton->setMaximumSize(QSize(25, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/cancel.ico"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon);

        horizontalLayout_8->addWidget(stopButton);

        startButton = new QPushButton(frame);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        sizePolicy8.setHeightForWidth(startButton->sizePolicy().hasHeightForWidth());
        startButton->setSizePolicy(sizePolicy8);
        startButton->setMaximumSize(QSize(25, 25));
        startButton->setBaseSize(QSize(25, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/start_copy.ico"), QSize(), QIcon::Normal, QIcon::Off);
        startButton->setIcon(icon1);

        horizontalLayout_8->addWidget(startButton);

        progressBar = new myprogressbar(frame);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy9(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy9);
        progressBar->setMinimumSize(QSize(0, 22));
        progressBar->setMaximumSize(QSize(16777215, 22));
        progressBar->setBaseSize(QSize(0, 22));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush1(QColor(0, 170, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Highlight, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Highlight, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        QBrush brush2(QColor(51, 153, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Highlight, brush2);
        progressBar->setPalette(palette1);
        progressBar->setMaximum(1000);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        horizontalLayout_8->addWidget(progressBar);


        verticalLayout_7->addLayout(horizontalLayout_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        sizeLabel = new QLabel(frame);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));
        sizePolicy5.setHeightForWidth(sizeLabel->sizePolicy().hasHeightForWidth());
        sizeLabel->setSizePolicy(sizePolicy5);
        sizeLabel->setMaximumSize(QSize(16777215, 16777215));
        QFont font6;
        font6.setBold(false);
        font6.setWeight(50);
        font6.setKerning(false);
        sizeLabel->setFont(font6);
        sizeLabel->setScaledContents(false);
        sizeLabel->setTextInteractionFlags(Qt::NoTextInteraction);

        horizontalLayout_5->addWidget(sizeLabel);

        ofLabel = new QLabel(frame);
        ofLabel->setObjectName(QString::fromUtf8("ofLabel"));
        sizePolicy5.setHeightForWidth(ofLabel->sizePolicy().hasHeightForWidth());
        ofLabel->setSizePolicy(sizePolicy5);
        ofLabel->setFont(font2);

        horizontalLayout_5->addWidget(ofLabel);

        totalSizeLabel = new QLabel(frame);
        totalSizeLabel->setObjectName(QString::fromUtf8("totalSizeLabel"));
        sizePolicy5.setHeightForWidth(totalSizeLabel->sizePolicy().hasHeightForWidth());
        totalSizeLabel->setSizePolicy(sizePolicy5);
        totalSizeLabel->setFont(font2);

        horizontalLayout_5->addWidget(totalSizeLabel);

        speedLabel = new QLabel(frame);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));
        sizePolicy5.setHeightForWidth(speedLabel->sizePolicy().hasHeightForWidth());
        speedLabel->setSizePolicy(sizePolicy5);
        speedLabel->setFont(font2);

        horizontalLayout_5->addWidget(speedLabel);

        fileCountLabel = new QLabel(frame);
        fileCountLabel->setObjectName(QString::fromUtf8("fileCountLabel"));
        sizePolicy5.setHeightForWidth(fileCountLabel->sizePolicy().hasHeightForWidth());
        fileCountLabel->setSizePolicy(sizePolicy5);
        fileCountLabel->setFont(font2);

        horizontalLayout_5->addWidget(fileCountLabel);

        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_7->addLayout(horizontalLayout_5);


        verticalLayout_2->addWidget(frame);


        verticalLayout->addWidget(groupBox);

        infoWidget = new QWidget(copyInfoPanel);
        infoWidget->setObjectName(QString::fromUtf8("infoWidget"));
        infoWidget->setEnabled(true);
        sizePolicy.setHeightForWidth(infoWidget->sizePolicy().hasHeightForWidth());
        infoWidget->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(infoWidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_2 = new QGroupBox(infoWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_2);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        cutRadioButton = new QRadioButton(groupBox_2);
        cutRadioButton->setObjectName(QString::fromUtf8("cutRadioButton"));

        horizontalLayout_7->addWidget(cutRadioButton);

        copyRadioButton = new QRadioButton(groupBox_2);
        copyRadioButton->setObjectName(QString::fromUtf8("copyRadioButton"));
        copyRadioButton->setChecked(true);

        horizontalLayout_7->addWidget(copyRadioButton);


        verticalLayout_3->addWidget(groupBox_2);

        checkSumCheckBox = new QCheckBox(infoWidget);
        checkSumCheckBox->setObjectName(QString::fromUtf8("checkSumCheckBox"));

        verticalLayout_3->addWidget(checkSumCheckBox);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label = new QLabel(infoWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_10->addWidget(label);

        speedSlider_inversed = new QSlider(infoWidget);
        speedSlider_inversed->setObjectName(QString::fromUtf8("speedSlider_inversed"));
        speedSlider_inversed->setMinimum(1);
        speedSlider_inversed->setMaximum(10);
        speedSlider_inversed->setPageStep(1);
        speedSlider_inversed->setValue(10);
        speedSlider_inversed->setSliderPosition(10);
        speedSlider_inversed->setOrientation(Qt::Horizontal);

        horizontalLayout_10->addWidget(speedSlider_inversed);

        speedLevelLabel = new QLabel(infoWidget);
        speedLevelLabel->setObjectName(QString::fromUtf8("speedLevelLabel"));

        horizontalLayout_10->addWidget(speedLevelLabel);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        editCheckBox = new QCheckBox(infoWidget);
        editCheckBox->setObjectName(QString::fromUtf8("editCheckBox"));
        QSizePolicy sizePolicy10(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(editCheckBox->sizePolicy().hasHeightForWidth());
        editCheckBox->setSizePolicy(sizePolicy10);
        editCheckBox->setFont(font2);

        horizontalLayout_6->addWidget(editCheckBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_6);

        editGroupBox = new QGroupBox(infoWidget);
        editGroupBox->setObjectName(QString::fromUtf8("editGroupBox"));
        QFont font7;
        font7.setBold(true);
        font7.setWeight(75);
        editGroupBox->setFont(font7);
        editGroupBox->setCheckable(true);
        verticalLayout_4 = new QVBoxLayout(editGroupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        addFileButton = new QPushButton(editGroupBox);
        addFileButton->setObjectName(QString::fromUtf8("addFileButton"));
        sizePolicy7.setHeightForWidth(addFileButton->sizePolicy().hasHeightForWidth());
        addFileButton->setSizePolicy(sizePolicy7);
        addFileButton->setMinimumSize(QSize(25, 25));
        addFileButton->setBaseSize(QSize(25, 25));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/add_file.ico"), QSize(), QIcon::Normal, QIcon::Off);
        addFileButton->setIcon(icon2);

        verticalLayout_5->addWidget(addFileButton);

        addFolderButton = new QPushButton(editGroupBox);
        addFolderButton->setObjectName(QString::fromUtf8("addFolderButton"));
        sizePolicy10.setHeightForWidth(addFolderButton->sizePolicy().hasHeightForWidth());
        addFolderButton->setSizePolicy(sizePolicy10);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/add_folder.ico"), QSize(), QIcon::Normal, QIcon::Off);
        addFolderButton->setIcon(icon3);

        verticalLayout_5->addWidget(addFolderButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        horizontalLayout_9->addLayout(verticalLayout_5);

        copyFileTree = new FileListWidget(editGroupBox);
        copyFileTree->setObjectName(QString::fromUtf8("copyFileTree"));
        sizePolicy.setHeightForWidth(copyFileTree->sizePolicy().hasHeightForWidth());
        copyFileTree->setSizePolicy(sizePolicy);
        copyFileTree->setAcceptDrops(true);

        horizontalLayout_9->addWidget(copyFileTree);


        verticalLayout_4->addLayout(horizontalLayout_9);


        verticalLayout_3->addWidget(editGroupBox);


        verticalLayout->addWidget(infoWidget);


        retranslateUi(copyInfoPanel);

        QMetaObject::connectSlotsByName(copyInfoPanel);
    } // setupUi

    void retranslateUi(QFrame *copyInfoPanel)
    {
        copyInfoPanel->setWindowTitle(QApplication::translate("copyInfoPanel", "NiceCopier Task", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        copyInfoPanel->setAccessibleName(QApplication::translate("copyInfoPanel", "CopyInfoPanel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        groupBox->setTitle(QApplication::translate("copyInfoPanel", "Preparing copy", 0, QApplication::UnicodeUTF8));
        fromLabel->setText(QApplication::translate("copyInfoPanel", "From: ", 0, QApplication::UnicodeUTF8));
        fromPathLabel->setText(QApplication::translate("copyInfoPanel", "C:From/this/path", 0, QApplication::UnicodeUTF8));
        targetLabel->setText(QApplication::translate("copyInfoPanel", "  To: ", 0, QApplication::UnicodeUTF8));
        targetPathLabel->setText(QApplication::translate("copyInfoPanel", "C:/To/This/Path/", 0, QApplication::UnicodeUTF8));
        fileLabel->setText(QApplication::translate("copyInfoPanel", "File: ", 0, QApplication::UnicodeUTF8));
        currentFileLabel->setText(QApplication::translate("copyInfoPanel", "C:/path/current file aaaaaaaaaaaaaa", 0, QApplication::UnicodeUTF8));
        fileSizeLabel->setText(QApplication::translate("copyInfoPanel", "100.100Gb", 0, QApplication::UnicodeUTF8));
        stopButton->setText(QString());
        startButton->setText(QString());
        sizeLabel->setText(QApplication::translate("copyInfoPanel", "100.100Mb", 0, QApplication::UnicodeUTF8));
        ofLabel->setText(QApplication::translate("copyInfoPanel", "of", 0, QApplication::UnicodeUTF8));
        totalSizeLabel->setText(QApplication::translate("copyInfoPanel", "100.100Mb", 0, QApplication::UnicodeUTF8));
        speedLabel->setText(QApplication::translate("copyInfoPanel", "100.10Kb/s", 0, QApplication::UnicodeUTF8));
        fileCountLabel->setText(QApplication::translate("copyInfoPanel", "( 100000 )", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("copyInfoPanel", "Copy Type", 0, QApplication::UnicodeUTF8));
        cutRadioButton->setText(QApplication::translate("copyInfoPanel", "Move", 0, QApplication::UnicodeUTF8));
        copyRadioButton->setText(QApplication::translate("copyInfoPanel", "Copy", 0, QApplication::UnicodeUTF8));
        checkSumCheckBox->setText(QApplication::translate("copyInfoPanel", "verify file checksum", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("copyInfoPanel", "Speed:  ", 0, QApplication::UnicodeUTF8));
        speedLevelLabel->setText(QApplication::translate("copyInfoPanel", "100%", 0, QApplication::UnicodeUTF8));
        editCheckBox->setText(QApplication::translate("copyInfoPanel", "Edit after preparing", 0, QApplication::UnicodeUTF8));
        editGroupBox->setTitle(QApplication::translate("copyInfoPanel", "Edit File list", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        addFileButton->setToolTip(QApplication::translate("copyInfoPanel", "Add file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addFileButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        addFolderButton->setToolTip(QApplication::translate("copyInfoPanel", "Add folder", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        addFolderButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class copyInfoPanel: public Ui_copyInfoPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPYINFOPANEL_H
