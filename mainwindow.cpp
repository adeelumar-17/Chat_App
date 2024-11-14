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
    //implementation of server
    TCPServer=new QTcpServer();
    if(TCPServer->listen(QHostAddress::Any,8080)){
        connect(TCPServer, &QTcpServer::newConnection, this, &MainWindow::newConnection);
        qDebug()<<"Server started successfully";
    }
    else{
        qDebug()<<"Server failed to start";
    }
    //connection to local host
    //(QHostAddress(x.x.x.x),1234) for remote host
    //implementation of client
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

void MainWindow::addNewClientConnection(QTcpSocket *socket)
{
    clientConnectionList.append(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readDataFromSocket);
    ui->clientsList->addItem(QString::number(socket->socketDescriptor());
    QString client="Client :"+QString::number(socket->socketDescriptor())+" connected with the server";
    ui->chatHistory->append(client);
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
    QString messageForClient=ui->msgData->text();
    QString receiver=ui->clientsList->currentText();
    if(ui->recipeintType->currentText()=="Everyone"){   //send to all connected clients
        foreach(QTcpSocket *socket,clientConnectionList){
                socket->write(messageForClient.toStdString().c_str());
        }
    }
    else{
        foreach(QTcpSocket *socket,clientConnectionList){     //send to selected clients only
            if(socket->socketDescriptor()==receiver.toLongLong())
            {
                socket->write(messageForClient.toStdString().c_str());

            }
        }
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

void MainWindow::readDataFromSocket()
{

}

void MainWindow::newConnection()
{
    while(TCPServer->hasPendingConnections()){
         addNewClientConnection(TCPServer->nextPendingConnection());
    }
}

