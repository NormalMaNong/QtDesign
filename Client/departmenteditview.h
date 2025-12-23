#ifndef DEPARTMENTEDITVIEW_H
#define DEPARTMENTEDITVIEW_H

#include <QWidget>

namespace Ui {
class DepartmentEditView;
}

class DepartmentEditView : public QWidget
{
    Q_OBJECT

public:
    explicit DepartmentEditView(QWidget *parent = nullptr);
    ~DepartmentEditView();

private:
    Ui::DepartmentEditView *ui;
};

#endif // DEPARTMENTEDITVIEW_H
