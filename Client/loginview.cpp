#include "loginview.h"
#include "ui_loginview.h"
#include <idatabase.h>
#include <QMessageBox>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
    m_chatClient = new ChatClient(this);
    connect(m_chatClient, &ChatClient::connected, this, &LoginView::connectToServer);
    connect(m_chatClient, &ChatClient::jsonReceived, this, &LoginView::jsonReceived);

}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_btSignUp_clicked()
{
    type = "登录";
    m_chatClient->connectToServer(QHostAddress("127.0.0.1"), 1967);
    if(loginFlag){
        emit loginSuccess();
        m_chatClient->disconnectFromHost();
        loginFlag = false;
    }
}

void LoginView::connectToServer()
{
    m_chatClient->sendMessage(ui->inputUserName->text(), ui->inputUserPassword->text(), type);
}

void LoginView::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if(typeVal.isNull() || !typeVal.isString())
        return;
    if(typeVal.toString().compare("登录成功") == 0){
        loginFlag = true;
    }else if(typeVal.toString().compare("注册成功") == 0){
        const QJsonValue usernameVal = docObj.value("username");
        if(usernameVal.isNull() || !usernameVal.isString())
            return;
        QMessageBox msgBox;
        msgBox.setWindowTitle("注册结果");
        msgBox.setText(typeVal.toString());
        msgBox.setInformativeText("用户 " + usernameVal.toString() + " 注册成功");
        msgBox.setDetailedText("用户名: " + usernameVal.toString());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        logonFlag = true;
    }else if(typeVal.toString().compare("注册失败") == 0){
        const QJsonValue reasonVal = docObj.value("reason");
        if(reasonVal.isNull() || !reasonVal.isString())
            return;
        QMessageBox msgBox;
        msgBox.setWindowTitle("错误报告");
        msgBox.setText(typeVal.toString());
        msgBox.setInformativeText("建议视错误原因重新操作");
        msgBox.setDetailedText(reasonVal.toString());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    m_chatClient->disconnectFromHost();
}

void LoginView::on_btSignIn_clicked()
{
    type = "注册";
    m_chatClient->connectToServer(QHostAddress("127.0.0.1"), 1967);
    if(logonFlag){
        logonFlag = false;
    }
}

