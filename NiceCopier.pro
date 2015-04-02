#-------------------------------------------------
#
# Project created by QtCreator 2010-11-13T16:29:58
#
#-------------------------------------------------
QT += core
QT += gui
QT += widgets
QT += network
QT += xml

CONFIG += static
CONFIG -= openssl-linked
CONFIG -= openssl
DEFINES += STATIC

TEMPLATE = app
TARGET = NiceCopier

DEFINES += _WIN32_WINNT=0x0501
QMAKE_LFLAGS = -static-libgcc -static-libstdc++ -static
LFLAGS = -static-libgcc -static-libstdc++ -static

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
    TaskThread/File.cpp \
    TaskThread/Folder.cpp \
    extensions/qxmlstreamattributesext.cpp \
    extensions/qstringext.cpp \
    FileListWidget.cpp \
    fancylineedit.cpp \
    filterlineedit.cpp \
    recenttasks.cpp \
    taskwidget.cpp \
    updater.cpp \
    tasksdialog.cpp


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
    memory.h \
    myprogressbar.h \
    TaskThread/File.h \
    TaskThread/Folder.h \
    RecentTasks.h \
    extensions/qxmlstreamattributesext.h \
    extensions/qstringext.h \
    filelistwidget.h \
    extensions/filelist.h \
    fancylineedit.h \
    filterlineedit.h \
    TaskWidget.h \
    TasksDialog.h \
    updater.h

FORMS    += nicecopier.ui \
    copyinfopanel.ui \
    filelist.ui \
    errormessages.ui \
    tasklist.ui \
    ncsettings.ui \
    shutdowntimerdialog.ui \
    tasks_dialog_t.ui \
    task_widget_t.ui


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



release: DESTDIR = build/release

contains(CONFIG,release64){
    message (64 bit build)
    message ($$PWD/src)
    release64: DESTDIR = build/release64
}

mydebug: DESTDIR = build/debug


MOC_DIR = $${DESTDIR}/moc
OBJECTS_DIR = $${DESTDIR}/obj
UI_DIR = $${DESTDIR}/ui

RESOURCES += \
    resource.qrc

RC_FILE = appico.rc

CONFIG(release, debug|release)    {
    message (release build)

   LIBS += -L$$[QT_INSTALL_PLUGINS]/imageformats -lqico
   LIBS += -L$$[QT_INSTALL_PLUGINS]/platforms -lqwindows

}

contains(CONFIG, mydebug){
    message (debug build)
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -g
    QMAKE_CFLAGS_RELEASE += -g
    QMAKE_LFLAGS_RELEASE = -g
}else{
    QMAKE_LFLAGS_RELEASE += -Wl,--gc-sections
}

LIBS += -lkernel32 -lAdvapi32 -lUser32
