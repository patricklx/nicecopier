


#include <QtGui/QApplication>
#include <qplugin.h>
#include <QString>
#include <QDir>
#include <QModelIndex>
#include <qxmlstream.h>
#include <QPlastiqueStyle>
#include <windows.h>
#include <QTextCodec>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <stdlib.h>
#include <Winbase.h>

#include "nicecopier.h"
#include "TaskThread/File.h"
#include "TaskThread/ReadableSize.h"
#include "extensions/qstringext.h"


/*some memory locking, only in the startup*/
/*
memory_t memory;


void* operator new[] (std::size_t size) throw (std::bad_alloc){return memory.operator_new(size);}

void* operator new[] (std::size_t size, const std::nothrow_t& nothrow_constant) throw(){return memory.operator_new(size);}

void* operator new (std::size_t size) throw (std::bad_alloc){return memory.operator_new(size);}

void* operator new (std::size_t size, const std::nothrow_t& nothrow_constant) throw(){return memory.operator_new(size);}



void operator delete (void* ptr) throw (){memory.operator_delete(ptr);}
void operator delete (void* ptr, const std::nothrow_t& nothrow_constant) throw(){memory.operator_delete(ptr);}
void operator delete[] (void* ptr) throw (){memory.operator_delete(ptr);}
void operator delete[] (void* ptr, const std::nothrow_t& nothrow_constant) throw(){memory.operator_delete(ptr);}
*/
#if defined(QT_NO_DEBUG)
Q_IMPORT_PLUGIN(qico)
#endif


void myMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
        break;
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
        break;
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
        break;
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
    }

    QFile outFile("log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}


int main(int argc,char **argv)
{
    QDir::setCurrent(QStringExt(argv[0]).beforeLast('\\'));
    qDebug("starting");
    QApplication *a = new QApplication(argc, argv);
   //qDebug("QApplication - pointers: %d",memory.pointers);
    QStringList cmdline_args = a->arguments();
    if( cmdline_args.contains("debug") )
    {
        qDebug("setting qMsgHandler");
        qInstallMsgHandler(myMessageHandler);
        qDebug("qMsgHandler set");
    }

    QFile outFile("log.txt");
    outFile.remove();
    a->setQuitOnLastWindowClosed(false);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    NiceCopier *w = new NiceCopier();
    a->exec();
    delete w;
    delete a;
    return 0;
}
/*

 made public: QModelIndexList selectedIndexes() const;


 added to xmlstream.h:
    void writeAttribute(const QString &qualifiedName,QByteArray &array)
    {
        writeAttribute(qualifiedName,QString(array));
    }

    void writeAttribute(const QString &qualifiedName,QVariant value)
    {
        writeAttribute(qualifiedName,value.toString());
    }

changes made to qstring.h:

    QString afterLast(QChar word)
    {
        int index = lastIndexOf(word)+1;
        return right(size()-index);
    }

    QString afterLast(QString word)
    {
        int index = lastIndexOf(word)+word.length();
        return right(size()-index);
    }

    QString beforeLast(QChar word)
    {
        int index = lastIndexOf(word);
        return left(index);
    }

    QString beforeLast(QString word)
    {
        int index = lastIndexOf(word);
        return left(index);
    }

    QString afterFirst(QString word)
    {
        int index = indexOf(word)+word.length();
        return right(size()-index);
    }
    QString afterFirst(QChar word)
    {
        int index = indexOf(word)+1;
        return right(size()-index);
    }

    QString beforeFirst(QString word)
    {
        int index = indexOf(word);
        return left(index);
    }
    QString beforeFirst(QChar word)
    {
        int index = indexOf(word);
        return left(index);
    }

qlocalserver_win.cpp
bool QLocalServerPrivate::addListener()
{
    // The old NULL dacl only allowed admins to connect for read/write.
    // Instead we create an empty dacl that allows all users to connect.

    // I guess this option should be configurable, e.g.  by an "unrestricted" property in QLocalServer.

    SECURITY_DESCRIPTOR secDesc;
    if (!::InitializeSecurityDescriptor(&secDesc, SECURITY_DESCRIPTOR_REVISION))
    {
        setError(QLatin1String("QLocalServerPrivate::InitializeSecurityDescriptor failed"));
        return false;
    }

    if (!::SetSecurityDescriptorDacl(&secDesc, TRUE, NULL, FALSE))
    {
        setError(QLatin1String("QLocalServerPrivate::SetSecurityDescriptorDacl failed"));
        return false;
    }

    SECURITY_ATTRIBUTES secAtt;
    secAtt.nLength = sizeof(SECURITY_ATTRIBUTES);
    secAtt.lpSecurityDescriptor = static_cast<PVOID>(&secDesc);
    secAtt.bInheritHandle = false;

    // The object must not change its address once the
    // contained OVERLAPPED struct is passed to Windows.
    listeners << Listener();
    Listener &listener = listeners.last();

    listener.handle = CreateNamedPipe(
                 (const wchar_t *)fullServerName.utf16(), // pipe name
                 PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,       // read/write access
                 PIPE_TYPE_BYTE |          // byte type pipe
                 PIPE_READMODE_BYTE |      // byte-read mode
                 PIPE_WAIT,                // blocking mode
                 PIPE_UNLIMITED_INSTANCES, // max. instances
                 BUFSIZE,                  // output buffer size
                 BUFSIZE,                  // input buffer size
                 3000,                     // client time-out
                 &secAtt);
*/
