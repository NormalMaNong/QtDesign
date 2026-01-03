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
    if(!patientTabModle->removeRow(curIndex.row())){
        qDebug() << "删除患者失败";
        return false;
    }
    if(!patientTabModle->submitAll()){
        qDebug() << "提交删除失败：" << patientTabModle->lastError().text();
        patientTabModle->revertAll();
        return false;
    }
    patientTabModle->select();
    return true;
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

bool IDataBase::searchDepartment(QString filter)
{
    departmentTabModle->setFilter(filter);
    return departmentTabModle->select();
}

bool IDataBase::deleteCurrentDepartment()
{
    QModelIndex curIndex = theDepartmentSelection->currentIndex();

    // 获取要删除的科室ID
    QSqlRecord curRec = departmentTabModle->record(curIndex.row());
    QString deptId = curRec.value("ID").toString();
    QString deptName = curRec.value("NAME").toString();

    if (deptName == "待分配科室") {
        qDebug() << "不能删除待分配科室！";
        return false;
    }

    // 查找待分配科室的ID
    QString defaultDeptId = getDefaultDepartmentId();


    QSqlQuery query(database);
    query.prepare("UPDATE doctor SET DEPARTMENT_ID = :DEFAULT_ID WHERE DEPARTMENT_ID = :DEPT_ID");
    query.bindValue(":DEFAULT_ID", defaultDeptId);
    query.bindValue(":DEPT_ID", deptId);

    if (!query.exec()) {
        qDebug() << "更新医生数据失败：" << query.lastError().text();
        return false;
    }

    if (!departmentTabModle->removeRow(curIndex.row())) {
        qDebug() << "删除科室失败";
        return false;
    }

    if (!departmentTabModle->submitAll()) {
        qDebug() << "提交删除失败：" << departmentTabModle->lastError().text();
        departmentTabModle->revertAll();
        return false;
    }

    if (doctorTabModle) {
        doctorTabModle->select();
    }

    departmentTabModle->select();

    return true;
}

bool IDataBase::submitDepartmentEdit()
{
    bool success = departmentTabModle->submitAll();
    if (!success) {
        qDebug() << "提交失败：" << database.lastError().text();
        qDebug() << "模型错误：" << departmentTabModle->lastError().text();
    }
    return success;
}

void IDataBase::revertDepartmentEdit()
{
    departmentTabModle->revertAll();
}

QString IDataBase::getDefaultDepartmentId()
{
    QSqlQuery query(database);
    query.prepare("SELECT ID FROM department WHERE NAME = '待分配科室'");

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return QString(); // 返回空字符串表示没找到
}

bool IDataBase::initDoctorModel()
{
    doctorTabModle = new QSqlTableModel(this, database);
    doctorTabModle->setTable("doctor");
    doctorTabModle->setEditStrategy(QSqlTableModel::OnManualSubmit);
    doctorTabModle->setSort(doctorTabModle->fieldIndex("name"),Qt::AscendingOrder);
    if(!(doctorTabModle->select()))
        return false;
    theDoctorSelection = new QItemSelectionModel(doctorTabModle);
    return true;
}

int IDataBase::addNewDoctor()
{
    doctorTabModle->insertRow(doctorTabModle->rowCount(),QModelIndex());
    QModelIndex curIndex = doctorTabModle->index(doctorTabModle->rowCount() - 1,1);

    int curRecNO = curIndex.row();
    QSqlRecord curRec = doctorTabModle->record(curRecNO);
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithBraces));
    curRec.setValue("EMPLOYEENO",QUuid::createUuid().toString(QUuid::WithBraces));
    doctorTabModle->setRecord(curRecNO,curRec);

    return curIndex.row();
}

bool IDataBase::searchDoctor(QString filter)
{
    doctorTabModle->setFilter(filter);
    return doctorTabModle->select();
}

bool IDataBase::deleteCurrentDoctor()
{
    QModelIndex curIndex = theDoctorSelection->currentIndex();
    if(!doctorTabModle->removeRow(curIndex.row())){
        qDebug() << "删除医生失败";
        return false;
    }
    if(!doctorTabModle->submitAll()){
        qDebug() << "提交删除失败：" << doctorTabModle->lastError().text();
        doctorTabModle->revertAll();
        return false;
    }
    doctorTabModle->select();
    return true;
}

bool IDataBase::submitDoctorEdit()
{
    bool success = doctorTabModle->submitAll();
    if (!success) {
        qDebug() << "提交失败：" << database.lastError().text();
        qDebug() << "模型错误：" << doctorTabModle->lastError().text();
    }
    return success;
}

void IDataBase::revertDoctorEdit()
{
    doctorTabModle->revertAll();
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
