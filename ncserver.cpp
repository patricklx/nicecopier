#include "ncserver.h"
#include <QMessageBox>
#include <QLocalServer>
#include <QLocalSocket>
#include <QBuffer>
#include <QApplication>
#include <ncsettings.h>
#include <QFile>
#include <windows.h>
#include <Aclapi.h>
#include <io.h>
#include <QDomDocument>
#include "extensions/qstringext.h"
#include <QTextCodec>

NCServer::NCServer()
    :QObject()
{
    nc_server.setSocketOptions(QLocalServer::OtherAccessOption);
    nc_server.listen("nicecopier_IPC");

    if( !nc_server.isListening())
    {
        QMessageBox::warning(NULL,tr("NiceCopier"),
                             tr("The server failed to start\n NiceCopier won't intercept any copy/move events!"));
    }else
    {
        connect(&nc_server,SIGNAL(newConnection()),SLOT(acceptNewClient()));
    }
}


void NCServer::acceptNewClient()
{
    QLocalSocket *client = nc_server.nextPendingConnection();
    if(client==NULL)
    {
        qWarning("NCServer: no client");
        return;
    }
    //connect(client,SIGNAL(disconnected()),SLOT(clientDisconnected()));
    qDebug("NCServer: client accepted");

    QByteArray received;


    while(!received.endsWith("\n\n") && client->state()!=QLocalSocket::UnconnectedState)
    {
        QByteArray buffer;
        client->waitForReadyRead(100);
        QApplication::processEvents();
        buffer = client->readAll();
        received += QString::fromWCharArray((const wchar_t*)buffer.constData(),buffer.length()/sizeof(wchar_t)).toLocal8Bit();
        if(NcSettings::isExiting())
            break;
    }

    if( !QString(received).endsWith("\n\n") ){

        qDebug() << received.length();
        qDebug() << received ;
        qDebug() << received.at(received.length()+1);

        qDebug() << "\ndoesnt end with \\n\\n";
        return;
    }

    qDebug("NCServer: client disconnected");
    qDebug() << received;
    if( !received.isEmpty() )
    {
        QDomDocument doc;
        QDomElement rootElement = doc.createElement("TASK_INFO");
        QList<QByteArray> lines = received.split('\n');
        lines.takeLast();lines.takeLast();
        if( lines.count()<3 ){
            return;
        }


        //read options
        QByteArray line = lines.takeFirst();
        QStringList options = QStringExt(line).afterFirst("OPTIONS:").split(';');
        foreach(QStringExt option,options){

            QString key = option.beforeFirst('=');
            QString value = option.afterFirst('=');
            rootElement.setAttribute(key,value);
        }

        //read target path
        line = lines.takeFirst();
        QDomElement targetElement = doc.createElement("DEST");
        QDomElement textElement = doc.createElement("path");
        QDomText textNode = doc.createTextNode(QStringExt(line).afterFirst("DEST:"));
        textElement.appendChild(textNode);
        targetElement.appendChild(textElement);

        //read sources
        QDomElement sourceElement = doc.createElement("SOURCE");
        while( !lines.empty() ){

            line = lines.takeFirst();
            QDomElement pathElement = doc.createElement("path");
            textNode = doc.createTextNode(line);
            pathElement.appendChild(textNode);

            sourceElement.appendChild(pathElement);
        }

        rootElement.appendChild(sourceElement);
        rootElement.appendChild(targetElement);


        doc.appendChild(rootElement);
        received = doc.toByteArray();
        qDebug() << received;

        QBuffer buffer;
        buffer.setBuffer(&received);
        buffer.open(QIODevice::ReadOnly);
        newCopyTask(buffer,client);
    }
    client->close();
    client->deleteLater();
}
