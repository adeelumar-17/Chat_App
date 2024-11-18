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

    // Implementation of server
    TCPServer = new QTcpServer(this);
    if (TCPServer->listen(QHostAddress::Any, 8080)) {
        connect(TCPServer, &QTcpServer::newConnection, this, &MainWindow::newConnection);
        qDebug() << "Server started successfully";
    } else {
        qDebug() << "Server failed to start";
    }

    // Connection to local host
    TCPSocket = new QTcpSocket(this);
    TCPSocket->connectToHost(QHostAddress::LocalHost, 8081);
    connect(TCPSocket, &QTcpSocket::readyRead, this, &MainWindow::readDataFromServer);

    if (TCPSocket->isOpen()) {
        qDebug() << "Connected to the server";
    } else {
        qDebug() << "Couldn't connect to the server";
    }
}

MainWindow::~MainWindow()
{
    TCPServer->close();
    for (QTcpSocket *client : clientConnectionList) {
        client->disconnectFromHost();
    }
    delete ui;
}

void MainWindow::appendMessage(const QString &message) {
    ui->chatHistory->append(message);
}

QString MainWindow::formatMessage(const QString &sender, const QString &message) {
    return sender + ": " + message;
}

void MainWindow::on_sendMsgButton_clicked()
{
    if (!TCPSocket || !TCPSocket->isOpen()) {
        QMessageBox::information(this, "Error", TCPSocket ? TCPSocket->errorString() : "Socket not initialized.");
        return;
    }

    QString writeData = "@" + Login::userID + ":" + ui->msgData->text();
    TCPSocket->write(writeData.toUtf8());
    appendMessage(writeData);

    QString messageForClient = ui->msgData->text();
    QString receiver = ui->clientsList->currentText();

    if (ui->recipeintType->currentText() == "Everyone") {
        for (QTcpSocket *socket : clientConnectionList) {
            socket->write(messageForClient.toUtf8());
        }
    } else {
        for (QTcpSocket *socket : clientConnectionList) {
            if (socket->socketDescriptor() == receiver.toLongLong()) {
                socket->write(messageForClient.toUtf8());
                break;
            }
        }
    }
}

void MainWindow::readDataFromServer()
{
    if (TCPSocket && TCPSocket->isOpen()) {
        QByteArray Data_From_Server = TCPSocket->readAll();
        QString serverMsg = QString::fromUtf8(Data_From_Server);
        appendMessage(serverMsg);
    }
}

void MainWindow::readDataFromSocket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray messageFromServer = socket->readAll();
    QString message = "Client " + QString::number(socket->socketDescriptor()) + " :: " + QString::fromUtf8(messageFromServer);
    appendMessage(message);
}

void MainWindow::newConnection()
{
    while (TCPServer->hasPendingConnections()) {
        QTcpSocket *newClient = TCPServer->nextPendingConnection();
        connect(newClient, &QTcpSocket::readyRead, this, &MainWindow::readDataFromSocket);
        connect(newClient, &QTcpSocket::disconnected, this, [=] { removeClient(newClient); });
        clientConnectionList.append(newClient);
        ui->clientsList->addItem(QString::number(newClient->socketDescriptor()));
        appendMessage("Client " + QString::number(newClient->socketDescriptor()) + " connected");
    }
}

void MainWindow::removeClient(QTcpSocket *socket)
{
    clientConnectionList.removeAll(socket);
    ui->clientsList->clear();  // Refresh clients list
    for (QTcpSocket *client : clientConnectionList) {
        ui->clientsList->addItem(QString::number(client->socketDescriptor()));
    }
    socket->deleteLater();
    appendMessage(QString("Client %1 disconnected").arg(socket->socketDescriptor()));
}
