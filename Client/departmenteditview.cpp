#include "departmenteditview.h"
#include "ui_departmenteditview.h"
#include <QSqlTableModel>
#include "idatabase.h"
#include <QDebug>
#include <QMessageBox>

DepartmentEditView::DepartmentEditView(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::DepartmentEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = IDataBase::getInstance().departmentTabModle;
    dataMapper->setModel(IDataBase::getInstance().departmentTabModle);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));

    dataMapper->setCurrentIndex(index);
}

DepartmentEditView::~DepartmentEditView()
{
    delete ui;
}

void DepartmentEditView::on_btSave_clicked()
{
    if (!dataMapper->submit()) {
        qDebug() << "提交到模型失败";
        return;
    }
    if (!IDataBase::getInstance().submitDepartmentEdit()) {
        qDebug() << "提交到数据库失败";
        return;
    }
    emit goPreviousView();
}


void DepartmentEditView::on_btCancel_clicked()
{
    IDataBase::getInstance().revertDepartmentEdit();
    emit goPreviousView();
}

