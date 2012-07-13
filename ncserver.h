#ifndef NCSERVER_H
#define NCSERVER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <new>

class NCServer : public QObject
{
        Q_OBJECT

    public:
        explicit NCServer();

    signals:
        void newCopyTask(QIODevice &device, QLocalSocket *client);

    private slots:
        void acceptNewClient();

    private:
        QLocalServer nc_server;

};

#endif // NCSERVER_H
