#include "doctoreditview.h"
#include "ui_doctoreditview.h"
#include <QSqlTableModel>
#include "idatabase.h"
#include <QDebug>
#include <QMessageBox>
#include <QTableView>

DoctorEditView::DoctorEditView(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::DoctorEditView)
{
    ui->setupUi(this);

    initDepartmentComboBox();

    dataMapper = new QDataWidgetMapper(this);
    QSqlTableModel *doctorModel = IDataBase::getInstance().doctorTabModle;
    dataMapper->setModel(doctorModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID, doctorModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, doctorModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditEmployeeID, doctorModel->fieldIndex("EMPLOYEENO"));
    dataMapper->addMapping(ui->dbEditDepartmentID, doctorModel->fieldIndex("DEPARTMENT_ID"));

    dataMapper->setCurrentIndex(index);

    // 设置当前科室选择
    setCurrentDepartment(index);
}

DoctorEditView::~DoctorEditView()
{
    delete ui;
}

void DoctorEditView::initDepartmentComboBox()
{
    deptModel = new QSqlTableModel(this, IDataBase::getInstance().database);
    deptModel->setTable("department");
    deptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    deptModel->setFilter("NAME != '待分配科室'");
    deptModel->select();

    ui->dbEditDepartmentID->setModel(deptModel);
    ui->dbEditDepartmentID->setModelColumn(deptModel->fieldIndex("ID"));

    ui->dbEditDepartmentID->setView(new QTableView());
    ui->dbEditDepartmentID->view()->setMinimumWidth(200);
}

void DoctorEditView::setCurrentDepartment(int index)
{
    // 获取当前医生的科室ID
    QSqlTableModel *doctorModel = IDataBase::getInstance().doctorTabModle;
    QModelIndex idx = doctorModel->index(index, doctorModel->fieldIndex("DEPARTMENT_ID"));
    QString currentDeptId = doctorModel->data(idx).toString();

    if (!currentDeptId.isEmpty()) {
        // 在科室模型中查找对应的科室ID
        for (int i = 0; i < deptModel->rowCount(); ++i) {
            QString deptId = deptModel->record(i).value("ID").toString();
            if (deptId == currentDeptId) {
                ui->dbEditDepartmentID->setCurrentIndex(i);
                break;
            }
        }
    }

}

void DoctorEditView::on_btSave_clicked()
{
    if (!dataMapper->submit()) {
        qDebug() << "提交到模型失败";
        QMessageBox::warning(this, "警告", "保存失败，请检查数据是否完整！");
        return;
    }
    if (!IDataBase::getInstance().submitDoctorEdit()) {
        qDebug() << "提交到数据库失败";
        QMessageBox::warning(this, "警告", "保存到数据库失败！");
        return;
    }

    emit goPreviousView();
}


void DoctorEditView::on_btCancel_clicked()
{
    IDataBase::getInstance().revertDoctorEdit();
    emit goPreviousView();
}

