#include "login.h"
#include "signup.h"
#include "mainwindow.h"
#include "ui_login.h"
#include "database.h"
QString Login::userID = "";
Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    installEventFilter(this);
}
Login::~Login()
{
    delete ui;
}
void Login::openMainWin(){
    //create main window dynamically
    MainWindow *mainWindow=new MainWindow();
    //show the main window
    mainWindow->show();
    //delete on close
    connect(mainWindow,&MainWindow::destroyed,mainWindow,&QObject::deleteLater);
    //close login page
    this->close();

}
void Login::openSignUp(){
    //create signup window dynamically
    SignUp *signup=new SignUp();
    //show the signup window
    signup->show();
    //delete on close
    connect(signup,&SignUp::destroyed,signup,&QObject::deleteLater);
    //close login page
    this->close();

}
bool Login::checkEmpty(const QString &id,const QString &pass){
    if(id.isEmpty() || pass.isEmpty()){
        if(id.isEmpty()){
            ui->userLabel->setText(QString::fromStdString("This field is required"));
            ui->userLabel->show();
        }
        if(pass.isEmpty()){
            ui->passLabel->setText(QString::fromStdString("This field is required"));
            ui->passLabel->show();
        }
        return false;
    }
    else
        return true;
}
bool Login::isValidLogin(const QString &uid,const QString &password){
    QSqlDatabase db = QSqlDatabase::database();  // Use the existing database connection
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return false;  // Ensure the database is open
    }

    QSqlQuery query;

    // SQL query to check for a matching username or email
    query.prepare("SELECT id, username, email, password FROM users WHERE username = :uid OR email = :uid");
    query.bindValue(":uid", uid);  // Bind the uid (either username or email)

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return false;
    }
    // If the user is found, compare the password
    if (query.next()) {
        QString storedPassword = query.value("password").toString(); // Get the stored password
        //retreive current user's username
        userID = query.value("username").toString();
        // Compare the input password with the stored password
        if (password == storedPassword) {
            qDebug() << "Login successful!";
            return true;
        } else {
            qDebug() << "Incorrect password!";
            ui->passLabel->setText(QString::fromStdString("Incorrect Password!"));
            ui->passLabel->show();
            return false;
        }
    } else {
        qDebug() << "No user found with the provided username or email!";
        ui->userLabel->setText(QString::fromStdString("No User Found!"));
        ui->userLabel->show();
        return false;
    }
}
bool Login::eventFilter(QObject *watched,QEvent *event){
    if (event->type() == QEvent::MouseButtonPress) {
        // If a mouse click occurs anywhere, hide the error message
        ui->userLabel->hide();
        ui->passLabel->hide();
        ui->userLabel->setText(QString::fromStdString("This field is required"));
        ui->passLabel->setText(QString::fromStdString("This field is required"));
    }
    // Pass the event to the parent class
    return QWidget::eventFilter(watched, event);
}
void Login::on_loginButton_clicked()
{
    userID=(Login::ui->idBox)->text();
    password=(Login::ui->passwordBox)->text();
    if(!checkEmpty(userID,password)){
        //do nothing
    }
    else if(!isValidLogin(userID,password)){
        //do nothing
    }
    else{
        openMainWin();
    }
}


void Login::on_signUpButton_clicked()
{
    openSignUp();
}

