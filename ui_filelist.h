/********************************************************************************
** Form generated from reading UI file 'filelist.ui'
**
** Created: Sat 4. Aug 21:12:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILELIST_H
#define UI_FILELIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "filterlineedit.h"

QT_BEGIN_NAMESPACE

class Ui_FileList
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *ckfullpath;
    FilterLineEdit *searchInput;
    QLabel *LbsearchResult;
    QSpacerItem *horizontalSpacer;
    QProgressBar *progressBar;
    QComboBox *cbtype;
    QTreeView *treeWidget;

    void setupUi(QWidget *FileList)
    {
        if (FileList->objectName().isEmpty())
            FileList->setObjectName(QString::fromUtf8("FileList"));
        FileList->resize(538, 226);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FileList->sizePolicy().hasHeightForWidth());
        FileList->setSizePolicy(sizePolicy);
        FileList->setAcceptDrops(true);
        verticalLayout = new QVBoxLayout(FileList);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, -1, -1, -1);
        ckfullpath = new QCheckBox(FileList);
        ckfullpath->setObjectName(QString::fromUtf8("ckfullpath"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ckfullpath->sizePolicy().hasHeightForWidth());
        ckfullpath->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(10);
        ckfullpath->setFont(font);

        horizontalLayout->addWidget(ckfullpath);

        searchInput = new FilterLineEdit(FileList);
        searchInput->setObjectName(QString::fromUtf8("searchInput"));

        horizontalLayout->addWidget(searchInput);

        LbsearchResult = new QLabel(FileList);
        LbsearchResult->setObjectName(QString::fromUtf8("LbsearchResult"));

        horizontalLayout->addWidget(LbsearchResult);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        progressBar = new QProgressBar(FileList);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(150, 0));
        progressBar->setMaximum(40000);
        progressBar->setValue(0);
        progressBar->setTextVisible(true);

        horizontalLayout->addWidget(progressBar);

        cbtype = new QComboBox(FileList);
        cbtype->setObjectName(QString::fromUtf8("cbtype"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cbtype->sizePolicy().hasHeightForWidth());
        cbtype->setSizePolicy(sizePolicy2);
        cbtype->setFont(font);

        horizontalLayout->addWidget(cbtype);


        verticalLayout->addLayout(horizontalLayout);

        treeWidget = new QTreeView(FileList);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(11);
        treeWidget->setFont(font1);
        treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        treeWidget->setAcceptDrops(true);
        treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget->setProperty("showDropIndicator", QVariant(false));
        treeWidget->setAlternatingRowColors(true);
        treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeWidget->setIconSize(QSize(16, 16));
        treeWidget->setTextElideMode(Qt::ElideRight);
        treeWidget->setIndentation(10);
        treeWidget->setRootIsDecorated(true);
        treeWidget->setUniformRowHeights(true);
        treeWidget->setItemsExpandable(true);
        treeWidget->setSortingEnabled(true);

        verticalLayout->addWidget(treeWidget);


        retranslateUi(FileList);

        QMetaObject::connectSlotsByName(FileList);
    } // setupUi

    void retranslateUi(QWidget *FileList)
    {
        FileList->setWindowTitle(QApplication::translate("FileList", "Form", 0, QApplication::UnicodeUTF8));
        ckfullpath->setText(QApplication::translate("FileList", "show with path", 0, QApplication::UnicodeUTF8));
        searchInput->setPlaceholderText(QApplication::translate("FileList", "search", 0, QApplication::UnicodeUTF8));
        LbsearchResult->setText(QString());
        progressBar->setFormat(QApplication::translate("FileList", "%v/%m", 0, QApplication::UnicodeUTF8));
        cbtype->clear();
        cbtype->insertItems(0, QStringList()
         << QApplication::translate("FileList", "All", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "Existing", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "Same Date and Size", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "Newer", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "Older", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "New", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "Ignored", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "Replaced", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FileList", "Renamed", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class FileList: public Ui_FileList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILELIST_H
