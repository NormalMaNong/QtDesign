#include "patienteditview.h"
#include "ui_patienteditview.h"
#include <QSqlTableModel>
#include "idatabase.h"
#include <QDebug>
#include <QMessageBox>

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
    if(ui->dbEditIDCard->text().length() != 18){
        QMessageBox::warning(this, tr("警告"), tr("身份证号必须为18位！"));
        return;
    }
    if(ui->dbEditMobile->text().length() != 11){
        QMessageBox::warning(this, tr("警告"), tr("手机号必须为11位！"));
        return;
    }
    calculateAge();
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

void PatientEditView::calculateAge()
{
    QDate birthDate = ui->dbDateEditDOB->date();
    QDate currentDate = QDate::currentDate();
    int age = currentDate.year() - birthDate.year();
    // 如果今年还没过生日，年龄减1
    if (currentDate.month() < birthDate.month() ||
        (currentDate.month() == birthDate.month() &&
         currentDate.day() < birthDate.day())) {
        age--;
    }

    ui->dbEditAge->setText(QString::number(age));
}

