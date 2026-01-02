#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include "loginview.h"
#include "welcomeview.h"
#include "doctorview.h"
#include "departmentview.h"
#include "patientview.h"
#include "patienteditview.h"
#include "departmenteditview.h"
#include "doctoreditview.h"

namespace Ui {
class MasterView;
}

class MasterView : public QWidget
{
    Q_OBJECT

public:
    explicit MasterView(QWidget *parent = nullptr);
    ~MasterView();
public slots:
    void goLoginView();
    void goWelcomeView();
    void goDoctorView();
    void goDoctorEditView();
    void goDepartmentEditView();
    void goDepartmentView();
    void goPatientView();
    void goPatientEditView(int rowNo);
    void goPreviousView();

private slots:
    void on_btBack_clicked();

    void on_btLogout_clicked();

    void on_stackedWidget_currentChanged(int arg1);

private:
    void pushWidgetToStackView(QWidget *widget);
    Ui::MasterView *ui;

    WelcomeView *welcomeView;
    DoctorView *doctorView;
    DoctorEditView *doctorEditView;
    PatientView *patientView;
    DepartmentView *departmentView;
    DepartmentEditView *departmentEditView;
    LoginView *loginView;
    PatientEditView *patientEditView;
};

#endif // MASTERVIEW_H
