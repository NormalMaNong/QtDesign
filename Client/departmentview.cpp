#include "departmentview.h"
#include "ui_departmentview.h"

DepartmentView::DepartmentView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DepartmentView)
{
    ui->setupUi(this);
}

DepartmentView::~DepartmentView()
{
    delete ui;
}

void DepartmentView::on_btAdd_clicked()
{
    emit goDepartmentEditView();
}

