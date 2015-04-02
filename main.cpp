
#undef min
#include <QApplication>
#include <QtGlobal>
#include <QString>
#include <QDir>
#include <QModelIndex>
#include <qxmlstream.h>
#include <windows.h>
#include <QTextCodec>
#include <QtDebug>
#include <QFile>
#include <qplugin.h>
#include <QTextStream>
#include <QStringList>
#include <stdlib.h>
#include <Winbase.h>
#include "nicecopier.h"
#include "TaskThread/File.h"
#include "TaskThread/ReadableSize.h"
#include "extensions/qstringext.h"
#include "ncsettings.h"
#include <QtPlugin>
#include <QStandardPaths>

QStringList filters;

#ifndef QT_DEBUG
Q_IMPORT_PLUGIN(QICOPlugin)
#endif


void myMessageHandler(QtMsgType type,const QMessageLogContext & , const QString &msg)
{
	QString txt(msg);

	bool ok=false;
	foreach(QString filter, filters){

		if(txt.contains(filter)){
			ok = true;
			break;
		}
	}

	if(filters.isEmpty())
		ok = true;

	if(!ok){
		return;
	}

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

    QApplication::setOrganizationName("winappmanager");
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir creator(".");
    creator.mkdir(dataDir);
    QDir::setCurrent(dataDir);

	QApplication a(argc, argv);
	a.setOrganizationName("NiceCopier");
	QStringList cmdline_args = a.arguments();
	if( cmdline_args.contains("-debug") )
	{
		if(cmdline_args.contains("-filters")){
			int index = cmdline_args.indexOf("-filters");
			QString filterlist = cmdline_args.at(index+1);
			filters = filterlist.split(",");
		}

		qDebug("main: setting qMsgHandler");
		qInstallMessageHandler(myMessageHandler);
		qDebug("main: qMsgHandler set");
	}


	QFile outFile("log.txt");
	outFile.remove();
	a.setQuitOnLastWindowClosed(false);

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	NiceCopier w(cmdline_args.contains("-hideicon"));

	a.exec();
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
qlocalserver_win.cpp
bool QLocalServerPrivate::addListener()
{
	// The old NULL dacl only allowed admins to connect for read/write.
	// Instead we create an empty dacl that allows all users to connect.

	// I guess this option should be configurable, e.g.  by an tr("unrestricted")ed") property in QLocalServer.

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

