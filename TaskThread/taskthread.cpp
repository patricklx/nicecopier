

#include "taskthread.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QMutexLocker>
#include <QtXml/QDomDocument>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QString>
#include <QDir>
#include<QSettings>

#include <stdio.h>
#include "tasklisthandler.h"

#include <windows.h>
#include <commctrl.h>



TaskThread::~TaskThread()
{
    //only save info if application is exiting and preparing is done
    if(prepareDone && forceExit)
    {
        totalFiles = directory->getToBeCopiedFilesCount();
    }
    //Delete info if the task was caneled or ended
    if ( !forceExit )
    {
        QString taskfile;
        taskfile.sprintf("Tasks/Task%i.Task",taskNr);
        QFile::remove(taskfile);
        taskfile.sprintf("Tasks/Task%i.TaskLastFile",taskNr);
        QFile::remove(taskfile);
    }
    else if(prepareDone)
    {
        saveAllInfo();
    }
    delete directory;
}

int TaskThread::getTaskId()
{
    return taskNr;
}

TaskThread::TaskThread(QIODevice &device, Config config  )
    : QThread(),fileCopyQueue(this)
{
    paused = false;
    shouldExit = false;
    prepareDone = false;
    copyOperationType = OpCopy;
    totalSizeDone = 0;
    totalSize = 0;
    totalFiles = 0;
    totalCpSize = 0;
    forceExit = false;
    loadFileSuccess = -1;
    QString from ;
    started = false;
    fileSizeToSave = 0;
    deleteDest = false;
    copyType = TaskListHandler::UNKNOWN;
    filesIgnored = false;
    edit = false;
    desktopPoint = QPoint(0,0);
    directory = NULL;
    checkMd5 = false;
    state = NotStarted;

    loadFileSuccess = loadFromXML( device, config );
    if ( loadFileSuccess != -1 )
    {
        if( config==ParseAndCopy ){

            foreach(QString source,sourceList){

                QFileInfo info(source);
                if(!info.exists()){
                    QMessageBox::warning(NULL,
                                         tr("Source not found"),
                                         tr("The source '")+source+tr("'was not found! Aborting"));
                    loadFileSuccess = -1;
                    return;
                }
            }
        }

        if ( sourceList.count()  > 0 )
        {

            from = sourceList[0];

            qDebug()<<"TaskThread: copy from: "<<from;

            QFileInfo f( from );
            QFileInfo d( destinationPath );

            if ( !f.isSymLink() && f.isDir() )
            {
                sourcePath = QStringExt(f.path()).beforeLast("/")+"/";

            }else
            {
                sourcePath = f.path();
                if( !sourcePath.endsWith("/") ){
                    sourcePath += "/";
                }
            }
            destinationPath = d.absoluteFilePath();
            if(config==ParseAndCopy){
                QFileInfo info(destinationPath);
                if(!info.exists()){
                    QMessageBox::warning(NULL,
                                         tr("Target not found"),
                                         tr("The target '")+destinationPath+tr("'was not found! Aborting"));
                    loadFileSuccess = -1;
                    return;
                }
            }

            destinationPath = Folder::getRealTargetPath(destinationPath);
            if(config==ParseAndCopy){
                QFileInfo info(destinationPath);
                if(!info.exists()){
                    QMessageBox::warning(NULL,
                                         tr("Target not found"),
                                         tr("The target '")+destinationPath+tr("'was not found! Aborting"));
                    loadFileSuccess = -1;
                    return;
                }
            }
        }
    }
    /*
    paths:
            F:/
            F:/file
            F:/dir/
            F:/dir/file
            //network/dir/
            //network/file
            //network/dir/file
*/
    if(directory==NULL)
        directory = new Folder(this,QFileInfo(sourcePath),QFileInfo(destinationPath));


    if( !resumeToThisFile.isEmpty() )
    {
        if(  !fileCopyQueue.resumeQueueToFile(resumeToThisFile) )
            QMessageBox::warning(NULL,"NiceCopier","resume failed");
    }

    if(destinationPath.startsWith("//"))
    {
        destVolume = destinationPath.afterFirst("//");
        destVolume = "//" + destVolume.beforeFirst('/');
    }else
    {
        destVolume = destinationPath.beforeFirst('/');
        destVolume.append('/');
    }


    qDebug()<<"TaskThread: source path: "<< sourcePath;
    if(sourcePath.startsWith("//"))
    {
        sourceVolume = sourcePath.afterFirst(QString("//"));
        qDebug()<<"TaskThread: sourceVolume: "<< sourceVolume;
        sourceVolume = "//" + sourceVolume.beforeFirst('/');
    }else
    {
        sourceVolume = sourcePath.beforeFirst('/');
        sourceVolume.append('/');
    }

    qDebug()<<"TaskThread: sourceVolume: "<< sourceVolume;


    currentSourceDir = sourcePath;
    currentDestDir = destinationPath;
}



bool TaskThread::shouldVerifyChecksum()
{
    return checkMd5;
}

void TaskThread::scheduleMd5Check(bool enable)
{
    checkMd5 = enable;
}

TaskThread::TaskState TaskThread::getState()
{
    return this->state;
}

QString TaskThread::getActFileError()
{
    if( fileCopyQueue.getCurrentFile() == NULL)
        return "";

    QString error;

    error = fileCopyQueue.getCurrentFile()->getDestinationError();
    if(!error.isEmpty())
        return "Destination: " + error;

    error = fileCopyQueue.getCurrentFile()->getSourceError();

    if(!error.isEmpty())
        return "Source: " + error;

    return "";
}

QString TaskThread::getSourcePath()
{
    return sourcePath;
}

QString TaskThread::getDestinationPath()
{
    return destinationPath;
}

Folder* TaskThread::getCopyDirectory()
{
    return directory;
}

TaskThread::CopyOperation TaskThread::getCopyOperation()
{
    return copyOperationType;
}

File* TaskThread::getCurrentFile() const
{
    return fileCopyQueue.getCurrentFile();
}

QString TaskThread::getCurrentFileName()
{
    return currentFileCopy;
}

QString TaskThread::getCurrentDestinationFolder()
{
    return currentDestDir;
}

QString TaskThread::getCurrentSourceFolder()
{
    return currentSourceDir;
}

QString TaskThread::getSourceVolume()
{
    return sourceVolume;
}

QString TaskThread::getTargetVolume()
{
    return destVolume;
}

double TaskThread::getActFileSize()
{
    if( fileCopyQueue.getCurrentFile() == NULL)
        return 1;

    return fileCopyQueue.getCurrentFile()->getSize();
}

double TaskThread::getActFileCpySize()
{
    if( fileCopyQueue.getCurrentFile() == NULL)
        return 1;
    return fileCopyQueue.getCurrentFile()->getCopiedSize();
}

void TaskThread::setCopyType( TaskListHandler::CopyType type )
{
    copyType = type;
    switch ( type )
    {
        case TaskListHandler::LOCAL_NETWORK:
            fileCopyQueue.setInicialBufferSize( 64 * 1024 );
        break;
        case TaskListHandler::FROM_INET:
            fileCopyQueue.setInicialBufferSize( 32 * 1024 );
        break;
        case TaskListHandler::TO_INET:
            fileCopyQueue.setInicialBufferSize( 32 * 1024 );
        break;
        case TaskListHandler::DISK_TO_DISK:
            fileCopyQueue.setInicialBufferSize( 1 * 1024 * 1024 );
        break;
        case TaskListHandler::FROM_CDDVD:
            fileCopyQueue.setInicialBufferSize( 512 * 1024 );
        break;
        default:
            fileCopyQueue.setInicialBufferSize( 512 * 1024 );
    }
}

void TaskThread::deleteDestination()
{
    Folder *folder = directory;


    foreach(Folder *subfolder,folder->getSubFolderList())
    {
        if( !subfolder->deleteDestination() )
            return;
    }

    foreach(File *f, folder->getFileList())
    {
        if( !f->deleteDest() )
            return;
    }
}

void TaskThread::deleteSource()
{
    if(filesIgnored)
    {
        int answer;

        QString text = tr("Some files where not copied!\n Do you want to delete the COPIED source files?");
        sendMessage(text,
                    &answer,
                    QMessageBox::Yes|QMessageBox::Abort);

        if( answer == QMessageBox::Abort )
        {
            return;
        }
    }
    Folder *folder = directory;

    foreach(Folder *subfolder,folder->getSubFolderList())
    {
        if( !subfolder->deleteSource() )
            return;
    }

    foreach(File *f, folder->getFileList())
    {
        if( !f->deleteSource() )
            return;
    }
}

void TaskThread::addTotalSize(double size)
{
    QMutexLocker lock(&infoMutex);
    totalSize += size;
}

void TaskThread::addSizeDone( double size)
{
    QMutexLocker lock(&infoMutex);
    totalSizeDone += size;
}

void TaskThread::addCopiedSize(double size)
{
    QMutexLocker lock(&infoMutex);
    totalCpSize += size;
}

void TaskThread::setTotalSize(double size)
{
    totalSize = size;
}


void TaskThread::setCurrentDirectory(QString source,QString dest)
{
    QMutexLocker lock(&infoMutex);
    if(!source.isEmpty())currentSourceDir = source;
    if(!dest.isEmpty())currentDestDir = dest;
}

void TaskThread::setCurrentFile( QString file )
{
    QMutexLocker lock(&infoMutex);
    currentFileCopy = file;
}

void TaskThread::addFileCount( int count )
{
    QMutexLocker lock(&infoMutex);
    totalFiles += count;
}

void TaskThread::updateFileSizeSave()
{
    fileSizeToSave = getTotalCopiedSize();
}

void TaskThread::setFilesIgnored()
{
    filesIgnored = true;
}

bool TaskThread::isPreparing()
{
    return !prepareDone;
}

bool TaskThread::isExiting()
{
    return shouldExit;
}

bool TaskThread::isDeleteDestinationScheduled()
{
    return deleteDest;
}

bool TaskThread::isEditEnabled()
{
    return edit;
}

void TaskThread::disableEdit()
{
    edit = false;
}

void TaskThread::scheduleDeleteDestination()
{
    deleteDest = true;
}

void TaskThread::exit(bool force)
{
    forceExit = force;
    shouldExit = true;
}

int TaskThread::isFileLoaded()
{
    return loadFileSuccess;
}

void TaskThread::pause()
{
    paused = true;
}

void TaskThread::resume()
{
    paused = false;
}

bool TaskThread::hasStarted()
{
    return started;
}

bool TaskThread::isPaused()
{
    return paused;
}

bool TaskThread::testDestroy()
{
    if ( paused && prepareDone ){
        saveAllInfo();
    }

    if( QThread::currentThread() == this )
    {
        while ( paused && !shouldExit )
            QThread::msleep(350);
    }

    return shouldExit;
}

void TaskThread::setItemPosition(QString item,int startIndex)
{
    HWND hDesktop = FindWindow(L"ProgMan", 0);
    hDesktop = GetWindow(hDesktop, GW_CHILD);
    hDesktop = GetWindow(hDesktop, GW_CHILD);

    DWORD explorer_pid;
    DWORD PROCESS_VM = PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_QUERY_INFORMATION;

    GetWindowThreadProcessId(hDesktop, &explorer_pid);
    HANDLE hExplorer = OpenProcess(PROCESS_VM, 0, explorer_pid);
    if(!hExplorer)
    {
        qDebug("TaskThread: can't find explorer.exe");
        return;
    }

    LVFINDINFO *ipc_lvi = (LVFINDINFO*) VirtualAllocEx(hExplorer, 0, sizeof(LVFINDINFO), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    wchar_t *ipc_text = (wchar_t*) VirtualAllocEx(hExplorer, 0, sizeof(wchar_t)*MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    LVFINDINFO info;
    info.flags = LVFI_STRING;
    info.psz = ipc_text;

    wchar_t text[MAX_PATH+1]=L"";
    item.toWCharArray(text);

    WriteProcessMemory(hExplorer,ipc_lvi,&info,sizeof(info),0);
    WriteProcessMemory(hExplorer,ipc_text,text,MAX_PATH * sizeof(wchar_t),0);

    int index = ListView_FindItem(hDesktop,-1,ipc_lvi);

    if(index == -1)
    {
        qDebug("TaskThread: can't find item: %s",item.toUtf8().data());
        return;
    }

    ListView_SetItemPosition(hDesktop,index,desktopPoint.x(),desktopPoint.y());

    VirtualFreeEx(hExplorer, ipc_lvi, 0, MEM_RELEASE);
    VirtualFreeEx(hExplorer, ipc_text, 0, MEM_RELEASE);
    CloseHandle(hExplorer);
}


void TaskThread::run()
{
    started = true;
    QThread::msleep(100);
    emit starting();

    if ( prepareDone == false )
    {
        this->state = TaskThread::Preparing;
        prepareFiles.create(this,sourcePath,destinationPath);

        if(prepareFiles.prepareCopy())
        {
            prepareDone = true;
            saveAllInfo();
        }
    }

    this->state = TaskThread::PrepareDone;

    if ( !shouldExit )
    {
        emit prepared();
    }

    if ( !testDestroy() )
    {
        msleep(50);
        this->state = TaskThread::Copy;
        fileCopyQueue.startCopyQueue();
        qDebug("TaskThread: copy done");
        this->state = TaskThread::CopyDone;
        QThread::msleep(50);
    }

    currentSourceDir = sourcePath;
    currentDestDir = destinationPath;
    currentFileCopy.clear();
    for(int i=0;i<sourceList.count();i++)
    {
        QStringExt file = sourceList[i];
        if(file.endsWith('/'))
            file = file.beforeLast('/');
        currentFileCopy = currentFileCopy + file.afterLast('/') + " ; ";
    }

    if ( !testDestroy() )
        emit copyDone();


    if ( shouldExit == false )
    {
        if ( copyOperationType == OpMove)
        {
            msleep(500);
            if ( !testDestroy() )
            {
                currentDestDir = destinationPath;
                this->state = TaskThread::RemovingSource;
                deleteSource();
            }
        }
    }

    if( shouldExit && !forceExit && deleteDest )
    {
        shouldExit = false;
        bool tmp_paused = paused;
        paused = false;
        msleep(500);
        this->state = TaskThread::RemovingTarget;
        deleteDestination();
        shouldExit = true;
        paused = tmp_paused;
    }
}

void TaskThread::xmlSaveFiles(QXmlStreamWriterExt &doc,Folder *folder)
{
    doc.writeStartElement("FOLDER");
    {
        doc.writeAttribute("source_name",folder->getSourceName());
        doc.writeAttribute("dest_name",folder->getDestName());
        doc.writeAttribute("dest_path",folder->getDestPath(Folder::RelativePath));
        doc.writeAttribute("source_path",folder->getSourcePath(Folder::RelativePath));
        doc.writeAttribute("copyhandle",folder->getCopyHandle());

        foreach(File *file, folder->getFileList())
        {
            doc.writeStartElement("FILE");
            {
                doc.writeAttribute("fSourceName",file->getSourceName());
                doc.writeAttribute("fDestName",file->getDestinationName().afterLast("/"));
                doc.writeAttribute("fsize",file->getSize());
                doc.writeAttribute("copyhandle",file->getCopyHandle());
                doc.writeAttribute("copied",file->wasCopied());
                doc.writeAttribute("copy_started",file->hasCopyStarted());
                doc.writeAttribute("exists",file->exists());
                doc.writeAttribute("destAge",file->compareAge());
                doc.writeAttribute("targetSize",file->getTargetSize());
            }
            doc.writeEndElement();
        }

        foreach(Folder *subfolder,folder->getSubFolderList())
        {
            xmlSaveFiles(doc,subfolder);
        }
    }
    doc.writeEndElement();
}

void TaskThread::saveAllInfo()
{
    QString size;
    size.setNum(totalSize);
    QString size_done;
    size_done.setNum(fileSizeToSave);
    QString nr_files;
    nr_files.setNum( totalFiles );
    QString startedStr;
    startedStr.setNum( started );

    qDebug("TaskThread: saving file info");

    QString taskfile;
    taskfile.sprintf("Tasks/Task%i.Task",taskNr);

    QFile file(taskfile);
    if( !file.open(QFile::WriteOnly) )
    {
        sendMessage(tr("Error: Can't open file \"") + taskfile +"\"" );
        return;
    }



    QXmlStreamWriterExt doc(&file);
    doc.setAutoFormatting(true);

    QString op;
    if ( copyOperationType == OpMove )
        op = "MOVE";
    else
        op = "COPY";

    doc.writeStartDocument();
    {
        doc.writeStartElement("TASK_INFO");
        {
            doc.writeAttribute( "TYPE", copyType );
            doc.writeAttribute( "PAUSED", paused );
            doc.writeAttribute( "STARTED", startedStr );
            if ( prepareDone )
            {
                doc.writeAttribute( "SIZE", size );
                doc.writeAttribute( "SIZE_DONE", size_done );
                doc.writeAttribute( "TOTAL_FILES", nr_files );
            }
            doc.writeAttribute( "OP", op );
            QString id;
            id.setNum( taskNr );
            doc.writeAttribute( "ID", id );
            if(edit)
                doc.writeAttribute("edit",tr("enabled"));


            doc.writeStartElement("DEST");
            {
                doc.writeTextElement("path",destinationPath);
            }
            doc.writeEndElement();


            size_t count, i = 0;
            count = sourceList.count();
            if ( count == 0 )
            {
                QFile::remove(taskfile);
                return;
            }

            doc.writeStartElement("SOURCE");
            while ( i < count )
            {
                QString source = sourceList[i];
                doc.writeTextElement("path",source);
                i++;
            }
            doc.writeEndElement();

            if ( prepareDone )
            {
                doc.writeStartElement("FILES");
                xmlSaveFiles(doc,directory);
                doc.writeEndElement();
            }
        }
        doc.writeEndElement();
    }
    doc.writeEndDocument();
}

int TaskThread::loadFromXML( QIODevice &device, bool no_save  )
{
    qDebug("TaskThread: loadFromXML");
    bool save = false;
    if(!device.isOpen())
        return -1;

    QXmlStreamReader doc(&device);
    doc.readNextStartElement();

    if ( doc.hasError() )
    {
        qWarning("TaskThread: Failed to load xml: %s\n\n %s",doc.errorString().toUtf8().data(),device.readAll().data() );
        return -1;
    }

    QXmlStreamAttributes attr = doc.attributes();

    totalSize = attr.value("SIZE").toString().toDouble();
    totalSizeDone = attr.value("SIZE_DONE").toString().toDouble();
    fileSizeToSave = totalSizeDone;
    totalFiles = attr.value("TOTAL_FILES").toString().toDouble();

    if(attr.hasAttribute("TYPE"))
        copyType =(TaskListHandler::CopyType) attr.value("TYPE").toString().toInt();
    else
        copyType = TaskListHandler::UNKNOWN;

    setCopyType( copyType );

    paused = QVariant(attr.value("PAUSED").toString()).toBool();

    started = QVariant(attr.value("STARTED").toString()).toBool();

    double id = attr.value("ID").toString().toDouble();
    qDebug("TaskThread: loaded id:%d",id);

    if( attr.value("edit")=="enabled")
        edit = true;

    QString fname;
    if ( id == 0 )
    {
        save = true;
        int _id;
        do
        {
            id = qrand();
            _id = id;
            fname.sprintf("Tasks/Task%i.Task",_id);
        }
        while ( QFile::exists( fname ) );
    }

    taskNr = id;
    qDebug("TaskThread: id is:%d",id);

    if ( attr.value("OP") == "COPY" )
        copyOperationType = OpCopy;
    else
        copyOperationType = OpMove;



    while(doc.readNextStartElement())
    {
        if ( doc.name() == "DEST" )
        {
            doc.readNextStartElement();
            doc.readNext();
            destinationPath = doc.text().toString();
            doc.readNextStartElement();//out of path element
            doc.readNextStartElement();//out of DEST element
        }

        if ( doc.name() == "SOURCE" )
        {
            while(doc.readNextStartElement())
            {
                doc.readNext();
                QStringExt file = doc.text().toString();
                file.replace("\\","/");
                sourceList.append( file );
                if(file.endsWith('/'))
                    file = file.beforeLast('/');
                currentFileCopy = currentFileCopy + file.afterLast('/') + " ; ";
                doc.readNextStartElement();
            }
        }

        if ( doc.name() == "FILES" )
        {
            QString fname;
            doc.readNextStartElement();
            {
                QXmlStreamAttributes attr = doc.attributes();
                directory = new Folder(this,NULL,attr);
                directory->load(doc);
            }
            doc.readNextStartElement();

            fname.sprintf("Tasks/Task%i.TaskLastFile",taskNr);
            if( QFile::exists( fname ) )
            {
                QFile txtoffset( fname );
                txtoffset.open(QFile::ReadOnly|QFile::Text);

                if ( txtoffset.isOpen() )
                {
                    resumeToThisFile = txtoffset.readLine();
                    resumeToThisFile = resumeToThisFile.beforeLast("\n");
                    QString filesizedone = txtoffset.readLine();
                    totalSizeDone += filesizedone.toDouble();

                    currentFileCopy = resumeToThisFile.afterLast('/');
                }
            }
        }

        //Set position of folders on the desktop
        if ( doc.name() == "POSITION" )
        {
            QXmlStreamAttributes attr = doc.attributes();
            double coord = attr.value( "x" ).toString().toDouble();
            desktopPoint.setX(coord);

            coord = attr.value( "y" ).toString().toDouble();
            desktopPoint.setY(coord);

            prepareFiles.setDirectoryPos( desktopPoint );
            fileCopyQueue.setFilePos( desktopPoint );
            doc.readNextStartElement();
        }
    }
    if (directory && (directory->getFileList().count() > 0 || directory->getSubFolderList().count() > 0 ))
        prepareDone = true;
    else
        prepareDone = false;

    if(doc.hasError())
    {
        QMessageBox::warning(NULL,"XML parse error",doc.errorString());
        return -1;
    }

    if(save && !no_save)
    {
        qDebug()<<"TaskThread: "<<"save info";
        saveAllInfo();
    }

    return taskNr;
}


void TaskThread::setSpeed(int speed)
{
    File::setSpeed(speed);
    TaskCopyQueue::setSpeed(speed);
}

double TaskThread::getTotalSize()
{
    return totalSize;
}

double TaskThread::getTotalFiles()
{
    return totalFiles;
}

/*
 *Inidicates the amount of copied bytes, including the size
 *of retrys.
*/
double TaskThread::getTotalCopiedSize()
{
    return totalCpSize;
}

/*
 *Indicates the number of copied bytes, excluding the size copied by retrys.
*/
double TaskThread::getFileSizeCopied()
{
    return totalSizeDone;
}

QList<QStringExt> TaskThread::getSourceList()
{
    return sourceList;
}

void TaskThread::updateTotalFiles()
{
    if( (this->state!=TaskThread::Copy
            && this->state!=PrepareDone
            && this->state!=TaskThread::Preparing)
            || this->deleteDest)
    {
        totalFiles = directory->getCopiedFilesCount();
    }else
    {
        totalFiles = directory->getToBeCopiedFilesCount();
    }
}

void TaskThread::aquireMutex()
{
    infoMutex.lock();
}

void TaskThread::releaseMutex()
{
    infoMutex.unlock();
}
