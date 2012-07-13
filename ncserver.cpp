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

NCServer::NCServer()
    :QObject()
{
    nc_server.listen("nicecopier_IPC");

    if( !nc_server.isListening())
    {
        QMessageBox::warning(NULL,"NiceCopier","The server failed to start\n NiceCopier won't intercept any copy/move events!");
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
	qWarning("no client");
        return;
    }
    //connect(client,SIGNAL(disconnected()),SLOT(clientDisconnected()));
    qDebug("client accepted");




    QByteArray recieved;

    while(!recieved.endsWith("\n\n") && client->state()!=QLocalSocket::UnconnectedState)
    {
        client->waitForReadyRead(100);
        QApplication::processEvents();
        recieved += client->readAll();
        if(NcSettings::isExiting())
            break;
    }

    qDebug("client disconnected");

    if( !recieved.isEmpty() )
    {
        QBuffer buffer;
        buffer.setBuffer(&recieved);
        buffer.open(QIODevice::ReadOnly);
        newCopyTask(buffer,client);
    }
    client->close();
    client->deleteLater();
}
