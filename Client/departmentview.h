#ifndef DEPARTMENTVIEW_H
#define DEPARTMENTVIEW_H

#include <QWidget>

namespace Ui {
class DepartmentView;
}

class DepartmentView : public QWidget
{
    Q_OBJECT

public:
    explicit DepartmentView(QWidget *parent = nullptr);
    ~DepartmentView();

private:
    Ui::DepartmentView *ui;

signals:
    void goDepartmentEditView(int idx);

private slots:
    void on_btAdd_clicked();
    void on_btSearch_clicked();
    void on_btDelete_clicked();
};

#endif // DEPARTMENTVIEW_H
