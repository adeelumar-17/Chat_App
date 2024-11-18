#include "signup.h"
#include "login.h"
#include "hashutil.h"
#include "database.h"
#include "ui_signup.h"

SignUp::SignUp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);
    installEventFilter(this);
}

SignUp::~SignUp()
{
    delete ui;
}
void SignUp::openLogin(){
    //create login window dynamically
    Login *login=new Login();
    //show the login window
    login->show();
    //delete on close
    connect(login,&Login::destroyed,login,&QObject::deleteLater);
    //close signup page
    this->close();

}
bool SignUp::checkEmpty(const QString &uName,const QString &email, const QString &pass, const QString &cPass){
    if(uName.isEmpty() || email.isEmpty() || pass.isEmpty() || cPass.isEmpty()){
        if(uName.isEmpty()){
            ui->userLabel->setText(QString::fromStdString("username is required"));
            ui->userLabel->show();
        }
        else{
            ui->userLabel->hide();
        }
        if(email.isEmpty()){
            ui->emailLabel->setText(QString::fromStdString("email is required"));
            ui->emailLabel->show();
        }
        else{
            ui->emailLabel->hide();
        }
        if(pass.isEmpty()){
            ui->passLabel->setText(QString::fromStdString("password is required"));
            ui->passLabel->show();
        }
        else{
            ui->passLabel->hide();
        }
        if(cPass.isEmpty()){
            ui->cPassLabel->setText(QString::fromStdString("confirm password"));
            ui->cPassLabel->show();
        }
        else{
            ui->cPassLabel->hide();
        }
        return false;
    }
    else
        return true;
}
bool SignUp::isValidEmail(const QString &email)
{
    // Regular expression pattern for a valid email
    QRegularExpression emailPattern("^[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}$");

    // Check if the email matches the pattern
    QRegularExpressionMatch match = emailPattern.match(email);

    if (match.hasMatch()) {
        return true;  // Email is valid
    } else {
        ui->emailLabel->setText(QString::fromStdString("enter a valid email"));
        ui->emailLabel->show();
        return false; // Email is invalid
    }
}
bool SignUp::eventFilter(QObject *watched,QEvent *event){
    if (event->type() == QEvent::MouseButtonPress) {
        // If a mouse click occurs anywhere, hide the error message
        ui->userLabel->hide();
        ui->emailLabel->hide();
        ui->passLabel->hide();
        ui->cPassLabel->hide();
        ui->userLabel->setText(QString::fromStdString("Username is required"));
        ui->emailLabel->setText(QString::fromStdString("Email is required"));
        ui->passLabel->setText(QString::fromStdString("Password is required"));
        ui->cPassLabel->setText(QString::fromStdString("Confirm password is required"));
    }
    // Pass the event to the parent class
    return QWidget::eventFilter(watched, event);
}
bool SignUp::matchPassword(const QString &pass, const QString &cPass){
    if(pass==cPass){
        return true;
    }
    else{
        ui->cPassLabel->setText(QString::fromStdString("Password doesn't match"));
        ui->cPassLabel->show();
        return false;
    }

}

void SignUp::storeData(const QString &uname, const QString &email, const QString &pass)
{
    // Assuming you have already established a connection to the database
    QSqlDatabase db = QSqlDatabase::database();  // Get the existing connection to the database

    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return;  // Ensure the database is open before proceeding
    }

    // Prepare the SQL query to insert user data into the 'users' table
    QSqlQuery query;
    QString hashedPassword=HashUtil::hashPassword(pass);
    // Use parameterized queries to prevent SQL injection
    query.prepare("INSERT INTO users (username, email, password) VALUES (:uname, :email, :pass)");

    // Bind the values to the placeholders
    query.bindValue(":uname", uname);
    query.bindValue(":email", email);
    query.bindValue(":pass", hashedPassword);  // Assuming you have hashed the password already

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Error inserting data: " << query.lastError().text();
    } else {
        qDebug() << "User data inserted successfully!";
    }
}
void SignUp::on_signUpButton_clicked()
{
    QString cPass;
    userName=(SignUp::ui->userNameBox)->text();
    email=(SignUp::ui->emailBox)->text();
    password=(SignUp::ui->passBox)->text();
    cPass=(SignUp::ui->cPassBox)->text();
    if(!checkEmpty(userName,email,password,cPass)){
        //do nothing
    }
    else if(!isValidEmail(email) || !matchPassword(password,cPass)){
        //do nothing
    }
    else{
        storeData(userName,email,password);
        openLogin();
    }
}

