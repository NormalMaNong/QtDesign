#include "patienteditview.h"
#include "ui_patienteditview.h"
#include <QSqlTableModel>
#include "idatabase.h"
#include <QDebug>

PatientEditView::PatientEditView(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = IDataBase::getInstance().patientTabModle;
    dataMapper->setModel(IDataBase::getInstance().patientTabModle);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard, tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbSpinHeight, tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbSpinWeight, tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditMobile, tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbDateEditDOB, tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbComboSex, tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbCreateTimeStamp, tabModel->fieldIndex("CREATETIMESTAMP"));
    dataMapper->addMapping(ui->dbEditAge, tabModel->fieldIndex("Age"));

    dataMapper->setCurrentIndex(index);
}

PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_btSave_clicked()
{
    if (!dataMapper->submit()) {
        qDebug() << "提交到模型失败";
        return;
    }
    if (!IDataBase::getInstance().submitPatientEdit()) {
        qDebug() << "提交到数据库失败";
        return;
    }
    emit goPreviousView();
}


void PatientEditView::on_btCancel_clicked()
{
    IDataBase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

