#include "departmentview.h"
#include "ui_departmentview.h"
#include "idatabase.h"

DepartmentView::DepartmentView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DepartmentView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);


    IDataBase &iDatabase = IDataBase::getInstance();
    if(iDatabase.initDepartmentModel()){
        ui->tableView->setModel(iDatabase.departmentTabModle);
        ui->tableView->setSelectionModel(iDatabase.theDepartmentSelection);
    }
}

DepartmentView::~DepartmentView()
{
    delete ui;
}

void DepartmentView::on_btAdd_clicked()
{
    int currow = IDataBase::getInstance().addNewDepartment();
    emit goDepartmentEditView(currow);
}

