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

bool IDataBase::initPatientModel()
{
    patientTabModle = new QSqlTableModel(this, database);
    patientTabModle->setTable("patient");
    patientTabModle->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientTabModle->setSort(patientTabModle->fieldIndex("name"),Qt::AscendingOrder);
    if(!(patientTabModle->select()))
        return false;
    thePatientSelection = new QItemSelectionModel(patientTabModle);
    return true;
}

int IDataBase::addNewPatient()
{
    patientTabModle->insertRow(patientTabModle->rowCount(),QModelIndex());
    QModelIndex curIndex = patientTabModle->index(patientTabModle->rowCount() - 1,1);

    int curRecNO = curIndex.row();
    QSqlRecord curRec = patientTabModle->record(curRecNO);
    curRec.setValue("CREATETIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithBraces));
    patientTabModle->setRecord(curRecNO,curRec);

    return curIndex.row();
}

bool IDataBase::searchPatient(QString filter)
{
    patientTabModle->setFilter(filter);
    return patientTabModle->select();
}

bool IDataBase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();
    patientTabModle->removeRow(curIndex.row());
    patientTabModle->submitAll();
    patientTabModle->select();
}

bool IDataBase::submitPatientEdit()
{
    bool success = patientTabModle->submitAll();
    if (!success) {
        qDebug() << "提交失败：" << database.lastError().text();
        qDebug() << "模型错误：" << patientTabModle->lastError().text();
    }
    return success;
}

void IDataBase::revertPatientEdit()
{
    patientTabModle->revertAll();
}

bool IDataBase::initDepartmentModel()
{
    departmentTabModle = new QSqlTableModel(this, database);
    departmentTabModle->setTable("department");
    departmentTabModle->setEditStrategy(QSqlTableModel::OnManualSubmit);
    departmentTabModle->setSort(departmentTabModle->fieldIndex("name"),Qt::AscendingOrder);
    if(!(departmentTabModle->select()))
        return false;
    theDepartmentSelection = new QItemSelectionModel(departmentTabModle);
    return true;
}

int IDataBase::addNewDepartment()
{
    departmentTabModle->insertRow(departmentTabModle->rowCount(),QModelIndex());
    QModelIndex curIndex = departmentTabModle->index(departmentTabModle->rowCount() - 1,1);

    int curRecNO = curIndex.row();
    QSqlRecord curRec = departmentTabModle->record(curRecNO);
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithBraces));
    departmentTabModle->setRecord(curRecNO,curRec);

    return curIndex.row();
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

IDataBase::IDataBase(QObject *parent)
    : QObject{parent}
{
    ininDataBase();
}
