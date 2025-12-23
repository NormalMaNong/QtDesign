#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private:
    Ui::LoginView *ui;

signals:
    void loginSuccess();
    void loginFailed();
private slots:
    void on_btSignUp_clicked();
};

#endif // LOGINVIEW_H
