#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"


class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    QVector<ServerWorker *>m_clients;
    bool validateUser(const QString &username, const QString &password);
    bool validateUserName(const QString &username);
    void Register(const QString &username, const QString &password);
    bool registerFlag = false;

signals:
    void logMessage(const QString &msg);


public slots:
    void stopServer();
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);
    void userDisconnected(ServerWorker *sender);

};

#endif // CHATSERVER_H
