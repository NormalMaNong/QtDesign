#ifndef DOCTOREDITVIEW_H
#define DOCTOREDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QSqlTableModel>

namespace Ui {
class DoctorEditView;
}

class DoctorEditView : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorEditView(QWidget *parent = nullptr, int index = 0);
    ~DoctorEditView();

private:
    Ui::DoctorEditView *ui;
    QDataWidgetMapper *dataMapper;
    QSqlTableModel *deptModel;

    void initDepartmentComboBox();
    void setCurrentDepartment(int index);

signals:
    void goPreviousView();
private slots:
    void on_btSave_clicked();
    void on_btCancel_clicked();
};

#endif // DOCTOREDITVIEW_H
