#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>

class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);

    QString userName();
    void setUserName(QString user);
    QString userPassword();
    void setUserPassword(QString pw);

signals:
    void logMessage(const QString &msg);
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);
    void disconnectFromClient();

private:
    QTcpSocket * m_serverSocket;
    QString m_userName;
    QString m_pw;

public slots:
    void onReadyRead();
    void sendJson(const QJsonObject &json);

};

#endif // SERVERWORKER_H
