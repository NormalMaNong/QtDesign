#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include "chatclient.h"

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

    ChatClient *m_chatClient;

    bool loginFlag = false;

signals:
    void loginSuccess();
    void loginFailed();
private slots:
    void on_btSignUp_clicked();

    void connectToServer();

    void jsonReceived(const QJsonObject &docObj);
};

#endif // LOGINVIEW_H
