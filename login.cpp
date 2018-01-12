#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QDebug>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->btn_login,SIGNAL(clicked()),this,SLOT(ClickedLoginBtn()));
    connect(ui->btn_logout,SIGNAL(clicked()),this,SLOT(ClickedLogoutBtn()));
    ui->user_name->setText(tr("sojo"));
    ui->password->setText(tr("300444"));
}

Login::~Login()
{
    delete ui;
}

void Login::ClickedLoginBtn()
{
    if(ui->user_name->text() == _user_name && ui->password->text() == _password)
    {
        accept();
    }
    else
    {
        QMessageBox::warning(NULL,"warning","please input right username or password");
    }
}

void Login::ClickedLogoutBtn()
{
    reject();
}

void Login::InitLoginInfo(QString init_user_name, QString init_password)
{
    _user_name = init_user_name;
    _password = init_password;
}
