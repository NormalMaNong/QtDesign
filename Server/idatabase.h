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

    QSqlDatabase database;

    void ininDataBase();
public:
    bool initPatientModel();
    int addNewPatient();
    bool searchPatient(QString filter);
    bool deleteCurrentPatient();
    bool submitPatientEdit();
    void revertPatientEdit();

    QSqlTableModel *patientTabModle;
    QItemSelectionModel *thePatientSelection;
signals:

private:


};

#endif // IDATABASE_H
