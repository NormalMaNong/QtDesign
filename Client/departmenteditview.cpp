#include "departmenteditview.h"
#include "ui_departmenteditview.h"

DepartmentEditView::DepartmentEditView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DepartmentEditView)
{
    ui->setupUi(this);
}

DepartmentEditView::~DepartmentEditView()
{
    delete ui;
}
