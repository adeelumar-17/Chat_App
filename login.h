#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QRegularExpression>
QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void openMainWin();
    void on_loginButton_clicked();
    bool checkEmpty(const QString &uid,const QString &pass);
    bool eventFilter(QObject *watched,QEvent *event)override;
    bool isValidLogin(const QString &uid,const QString &password);
    void openSignUp();
    void on_signUpButton_clicked();

private:
    Ui::Login *ui;
    QString userID;
    QString password;
};
#endif // LOGIN_H
