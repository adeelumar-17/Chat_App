#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
void MainWindow::setupDatabase() {
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/user_credentials.db";
    QDir().mkpath(QFileInfo(dbPath).path());  // Ensure directory exists

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Error: Unable to open the database!" << db.lastError().text();
    } else {
        qDebug() << "Database opened successfully at" << dbPath;
    }
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connection to local host
    //(QHostAddress(x.x.x.x),1234) for remote host
    TCPSocket=new QTcpSocket();
    TCPSocket->connectToHost(QHostAddress::LocalHost,8080);
    connect(TCPSocket, &QTcpSocket::readyRead, this, &MainWindow::readDataFromServer);
    TCPSocket->open(QIODevice::ReadWrite);
    if(TCPSocket->isOpen()){
        qDebug()<<"Connected to the server";
    }
    else{
        qDebug()<<"Couldn't connect to the server";
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::appendMessage(const QString &message){
    // Add the message to the chat history box with a new line
    ui->chatHistory->append(message);
}
//write data to the server
void MainWindow::on_sendMsgButton_clicked()
{
    if(TCPSocket){
        if(TCPSocket->isOpen()){
            QString writeData=QString("@")+Login::userID+QString(":");
            writeData+=ui->msgData->text();
            TCPSocket->write(writeData.toUtf8());
            appendMessage(writeData);
        }
        else{
            QMessageBox::information(this,"Error",TCPSocket->errorString());
        }
    }
    else{
        QMessageBox::information(this,"Error",TCPSocket->errorString());
    }
}
//read data from the server
void MainWindow::readDataFromServer()
{
    if (TCPSocket && TCPSocket->isOpen()) {
        QByteArray Data_From_Server = TCPSocket->readAll();
        QString serverMsg = QString::fromUtf8(Data_From_Server);
        appendMessage(serverMsg);  // Display message in chat box
    }
}

