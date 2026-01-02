#include "idatabase.h"
#include <QUuid>

void IDataBase::ininDataBase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "E:/Qt project/design/QtDesign.db";//使用绝对路径
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug() << "打不开数据库";
    }else
        qDebug() << "打开数据库";
}

bool IDataBase::initUserModel()
{
    userTabModle = new QSqlTableModel(this, database);
    userTabModle->setTable("user");
    userTabModle->setEditStrategy(QSqlTableModel::OnManualSubmit);
    userTabModle->setSort(userTabModle->fieldIndex("name"),Qt::AscendingOrder);
    if(!(userTabModle->select()))
        return false;
    theUserSelection = new QItemSelectionModel(userTabModle);
    return true;
}

int IDataBase::addNewUser()
{
    userTabModle->insertRow(userTabModle->rowCount(),QModelIndex());
    QModelIndex curIndex = userTabModle->index(userTabModle->rowCount() - 1,1);

    int curRecNO = curIndex.row();
    QSqlRecord curRec = userTabModle->record(curRecNO);
    userTabModle->setRecord(curRecNO,curRec);

    return curIndex.row();
}

bool IDataBase::submitUserEdit()
{
    bool success = userTabModle->submitAll();
    if (!success) {
        qDebug() << "提交失败：" << database.lastError().text();
        qDebug() << "模型错误：" << userTabModle->lastError().text();
    }
    return success;
}

QString IDataBase::userLogin(QString userName, QString password)
{
    QSqlQuery query;
    query.prepare("select username,password from user where username = :USER");
    query.bindValue(":USER", userName);
    query.exec();
    if(query.first() && query.value("username").isValid()){
        QString pw = query.value("password").toString();
        if(pw == password)
            return "loginOK";
        else{
            qDebug() << "密码错误";
            return "wrongPassword";
        }
    }else{
        qDebug() << "没有该用户";
        return "wrongUsername";
    }
}

QString IDataBase::userRegister(QString userName)
{
    QSqlQuery query;
    query.prepare("select username from user where username = :USER");
    query.bindValue(":USER", userName);
    query.exec();
    if(query.first() && query.value("username").isValid()){
        return "用户名已存在";
    }
    return "用户名可用";
}

IDataBase::IDataBase(QObject *parent)
    : QObject{parent}
{
    ininDataBase();
}
