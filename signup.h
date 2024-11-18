#ifndef SIGNUP_H
#define SIGNUP_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
namespace Ui {
class SignUp;
}

class SignUp : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private slots:
    bool checkEmpty(const QString &uName,const QString &email, const QString &pass, const QString &cPass);
    bool eventFilter(QObject *watched,QEvent *event)override;
    bool isValidEmail(const QString &email);
    void on_signUpButton_clicked();
    void openLogin();
    bool matchPassword(const QString &pass, const QString &cPass);
    void storeData(const QString &uname, const QString &email, const QString &pass);

private:
    Ui::SignUp *ui;
    QString userName;
    QString email;
    QString password;
};

#endif // SIGNUP_H
