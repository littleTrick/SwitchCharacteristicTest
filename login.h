#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void InitLoginInfo(QString init_user_name, QString init_password);

private slots:
    void ClickedLoginBtn();
    void ClickedLogoutBtn();
private:
    Ui::Login *ui;
    QString _user_name;
    QString _password;

};

#endif // LOGIN_H
