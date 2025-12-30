#include "loginview.h"
#include "ui_loginview.h"
#include <idatabase.h>

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
    m_chatClient->connectToServer(QHostAddress("127.0.0.1"), 1967);
    if(loginFlag){
        emit loginSuccess();
        m_chatClient->disconnectFromHost();
        loginFlag = false;
    }
}

void LoginView::connectToServer()
{
    m_chatClient->sendMessage(ui->inputUserName->text(), ui->inputUserPassword->text(), "登录");
}

void LoginView::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if(typeVal.isNull() || !typeVal.isString())
        return;
    if(typeVal.toString().compare("登录成功") == 0){
        loginFlag = true;
    }
}

