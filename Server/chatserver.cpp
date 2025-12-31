#include "chatserver.h"
#include "serverworker.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include "idatabase.h"
#include <QSqlTableModel>
#include <QDebug>
#include <QUuid>

ChatServer::ChatServer(QObject *parent):QTcpServer(parent) {}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if(!worker->setSocketDescriptor(socketDescriptor)){
        worker->deleteLater();
        return;
    }
    connect(worker, &ServerWorker::logMessage, this, &ChatServer::logMessage);
    connect(worker, &ServerWorker::jsonReceived, this, &ChatServer::jsonReceived);
    connect(worker, &ServerWorker::disconnectFromClient, this,
            std::bind(&ChatServer::userDisconnected, this, worker));
    m_clients.append(worker);
    emit logMessage("新用户连接上了");
}

bool ChatServer::validateUser(const QString &username, const QString &password)
{
    QString status = IDataBase::getInstance().userLogin(username,password);
    if(status == "loginOK")
        return true;
    else
        return false;
}

bool ChatServer::validateUserName(const QString &username)
{
    QString status = IDataBase::getInstance().userRegister(username);
    if(status == "用户名已存在")
        return false;
    else
        return true;
}

void ChatServer::Register(const QString &username, const QString &password)
{
    IDataBase &iDatabase = IDataBase::getInstance();
    if(iDatabase.initUserModel()){
        qDebug() << "数据库模型初始化成功";
    }
    QSqlTableModel *model = IDataBase::getInstance().userTabModle;
    if(!model){
        qDebug() << "数据库模型为空！";
        return;
    }
    int row = model->rowCount();
    if(!model->insertRow(row)){
        qDebug() << "插入行失败：" << model->lastError().text();
        return;
    }

    model->setData(model->index(row, model->fieldIndex("ID")), QUuid::createUuid().toString(QUuid::WithBraces));
    model->setData(model->index(row, model->fieldIndex("FULLNAME")), username);
    model->setData(model->index(row, model->fieldIndex("USERNAME")), username);
    model->setData(model->index(row, model->fieldIndex("PASSWORD")), password);

    if (!model->submit()) {
        qDebug() << "提交到模型失败";
        return;
    }
    if (!IDataBase::getInstance().submitUserEdit()) {
        qDebug() << "提交到数据库失败";
        return;
    }
    registerFlag = true;
}

void ChatServer::stopServer()
{
    close();
}

void ChatServer::jsonReceived(ServerWorker *sender, const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if(typeVal.isNull() || !typeVal.isString())
        return;
    if(typeVal.toString().compare("登录") == 0){
        const QJsonValue userNameVal = docObj.value("text");
        if(userNameVal.isNull() || !userNameVal.isString()){
            return;
        }
        const QJsonValue userPwVal = docObj.value("pw");
        if(userPwVal.isNull() || !userPwVal.isString()){
            return;
        }
        const QString username = userNameVal.toString();
        const QString pw = userPwVal.toString();
        bool flag = validateUser(username,pw);
        if(flag){
            QJsonObject successMessage;
            successMessage["type"] = "登录成功";
            successMessage["username"] = username;
            sender->sendJson(successMessage);
            emit logMessage(username + " 登录成功");
        }else{
            QJsonObject failMessage;
            failMessage["type"] = "登录失败";
            failMessage["reason"] = "账号或密码错误";
            sender->sendJson(failMessage);
            emit logMessage(username + " 登录失败");
            userDisconnected(sender);
        }
    }else if(typeVal.toString().compare("注册") == 0){
        const QJsonValue userNameVal = docObj.value("text");
        if(userNameVal.isNull() || !userNameVal.isString()){
            return;
        }
        const QJsonValue userPwVal = docObj.value("pw");
        if(userPwVal.isNull() || !userPwVal.isString()){
            return;
        }
        const QString username = userNameVal.toString();
        const QString pw = userPwVal.toString();
        bool flag = validateUserName(username);
        if(flag){
            Register(username,pw);
            if(registerFlag){
                QJsonObject successMessage;
                successMessage["type"] = "注册成功";
                successMessage["username"] = username;
                sender->sendJson(successMessage);
                emit logMessage(username + " 注册成功");
            }else{
                QJsonObject failMessage;
                failMessage["type"] = "注册失败";
                failMessage["reason"] = "同步到数据库失败";
                sender->sendJson(failMessage);
                emit logMessage(username + " 注册失败");
            }
        }else{
            QJsonObject failMessage;
            failMessage["type"] = "注册失败";
            failMessage["reason"] = "账号已存在";
            sender->sendJson(failMessage);
            emit logMessage(username + " 注册失败");
        }
        // userDisconnected(sender);
    }
}

void ChatServer::userDisconnected(ServerWorker *sender)
{
    m_clients.removeAll(sender);
    const QString userName = sender->userName();
    if(!userName.isEmpty()){
        QJsonObject disconnectedMessage;
        disconnectedMessage["type"] = "userdisconnected";
        disconnectedMessage["username"] = userName;
        emit logMessage(userName + " 下线了");
    }
    sender->deleteLater();
}


