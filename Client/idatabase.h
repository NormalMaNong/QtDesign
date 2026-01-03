#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDataBase : public QObject
{
    Q_OBJECT
public:

    static IDataBase& getInstance()
    {
        static IDataBase    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    QString userLogin(QString userName, QString password);

private:
    explicit IDataBase(QObject *parent = nullptr);
    IDataBase(IDataBase const&)               = delete;
    void operator=(IDataBase const&)  = delete;

    void ininDataBase();
public:
    bool initPatientModel();
    int addNewPatient();
    bool searchPatient(QString filter);
    bool deleteCurrentPatient();
    bool submitPatientEdit();
    void revertPatientEdit();
    bool initDepartmentModel();
    int addNewDepartment();
    bool searchDepartment(QString filter);
    bool deleteCurrentDepartment();
    bool submitDepartmentEdit();
    void revertDepartmentEdit();
    bool initDoctorModel();
    int addNewDoctor();
    bool searchDoctor(QString filter);
    bool deleteCurrentDoctor();
    bool submitDoctorEdit();
    void revertDoctorEdit();

    QSqlTableModel *patientTabModle;
    QItemSelectionModel *thePatientSelection;
    QSqlTableModel *departmentTabModle;
    QItemSelectionModel *theDepartmentSelection;
    QSqlTableModel *doctorTabModle;
    QItemSelectionModel *theDoctorSelection;

    QSqlDatabase database;
signals:

private:


};

#endif // IDATABASE_H
