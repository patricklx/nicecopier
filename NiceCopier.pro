#-------------------------------------------------
#
# Project created by QtCreator 2010-11-13T16:29:58
#
#-------------------------------------------------


QT       += core gui
QT       += network
QT       += xml

TARGET = NiceCopier
TEMPLATE = app




SOURCES +=\
    main.cpp\
    nicecopier.cpp \
    copyinfopanel.cpp \
    mytaskbaricon.cpp \
    errormessages.cpp \
    tasklist.cpp \
    TaskThread/taskthread.cpp \
    TaskThread/taskpreparer.cpp \
    TaskThread/taskcopyqueue.cpp \
    tasklisthandler.cpp \
    ncserver.cpp \
    ncsettings.cpp \
    treeitem.cpp \
    shutdowntimerdialog.cpp \
    recent_tasks_t.cpp \
    tasks_dialog_t.cpp \
    task_widget_t.cpp \
    updater_t.cpp \
    TaskThread/File.cpp \
    TaskThread/Folder.cpp \
    extensions/qxmlstreamattributesext.cpp \
    extensions/qstringext.cpp \
    FileListWidget.cpp \
    fancylineedit.cpp \
    filterlineedit.cpp


HEADERS  += nicecopier.h \
    copyinfopanel.h \
    mytaskbaricon.h \
    errormessages.h \
    tasklist.h \
    TaskThread/taskthread.h \
    TaskThread/taskpreparer.h \
    TaskThread/taskcopyqueue.h \
    qlabeleli.h \
    tasklisthandler.h \
    DiskSpace.h \
    TaskThread/ReadableSize.h \
    ncserver.h \
    ncsettings.h \
    treeitem.h \
    shutdowntimerdialog.h \
    tasks_dialog_t.h \
    memory.h \
    updater_t.h \
    myprogressbar.h \
    TaskThread/File.h \
    TaskThread/Folder.h \
    RecentTasks.h \
    TaskWidget_t.h \
    extensions/qxmlstreamattributesext.h \
    extensions/qstringext.h \
    filelistwidget.h \
    extensions/filelist.h \
    fancylineedit.h \
    filterlineedit.h

FORMS    += nicecopier.ui \
    copyinfopanel.ui \
    filelist.ui \
    errormessages.ui \
    tasklist.ui \
    ncsettings.ui \
    shutdowntimerdialog.ui \
    tasks_dialog_t.ui \
    task_widget_t.ui

DEFINES += MY_EXTENSION
QMAKE_CFLAGS_RELEASE= -O -O3 -pedantic -Wall -Wextra
QMAKE_CXXFLAGS_RELEASE = -O -O3
QMAKE_CXXFLAGS += -Wuninitialized -Wall -Wextra


RESOURCES += \
    resource.qrc

    RC_FILE = appico.rc

    CONFIG(release, debug|release)    {
        message (release build)
       CONFIG += static
       LIBS += -L$$[QT_INSTALL_PLUGINS]/imageformats -lqico
       QMAKE_LFLAGS += -static-libgcc
    }

OTHER_FILES += \
    appico.rc \
    icons/start_copy.ico \
    icons/retry.ico \
    icons/rename.ico \
    icons/pause_copy-old.ico \
    icons/pause_copy2.ico \
    icons/pause_copy.ico \
    icons/ignore.ico \
    icons/folder_delete.ico \
    icons/expand.ico \
    icons/delete_file.ico \
    icons/Copy2.ico \
    icons/copy.png \
    icons/Copy.ico \
    icons/contract.ico \
    icons/check.ico \
    icons/cancel_copy.ico \
    icons/cancel.ico \
    icons/add_folder.ico \
    icons/add_file.ico \
    language_pt.ts \
    language_de.ts \
    icons/find/wrapindicator.png \
    icons/find/wordandcase.png \
    icons/find/wholewords.png \
    icons/find/regexp.png \
    icons/find/magnifier.png \
    icons/find/editclear.png \
    icons/find/casesensitively.png \
    languages/language_pt.ts \
    languages/language_de.ts

 TRANSLATIONS    = languages/language_de.ts \
		   languages/language_pt.ts

 CODECFORTR      = ISO-8859-5

message(qt-version: $$[QT_VERSION])































































