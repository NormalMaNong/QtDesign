#ifndef DOCTORVIEW_H
#define DOCTORVIEW_H

#include <QWidget>

namespace Ui {
class DoctorView;
}

class DoctorView : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorView(QWidget *parent = nullptr);
    ~DoctorView();

private:
    Ui::DoctorView *ui;

signals:
    void goDoctorEditView();

private slots:
    void on_btAdd_clicked();
};

#endif // DOCTORVIEW_H
