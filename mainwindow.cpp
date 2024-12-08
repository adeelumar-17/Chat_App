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
    ui->msgData->setFocus();
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
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && event->modifiers()==Qt::ShiftModifier) {
        ui->sendMsgButton->click();
    }
   else{
        QMainWindow::keyPressEvent(event);
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

void MainWindow::appendMessage(const QString &message, bool isSentByUser) {
    // Define the alignment style based on the sender
    QString alignmentStyle = isSentByUser ? "text-align: right;" : "text-align: left;";

    // Wrap the message in a div to control alignment
    QString html = QString(
                       "<div style='%1; margin: 5px;'>"
                       "%2"
                       "</div>").arg(alignmentStyle).arg(message);

    // Append the styled message to QTextEdit
    ui->chatHistory->append(html);
}

void MainWindow::appendMessage(const QString &message, const QString &fileName, bool isSentByUser) {
    QFileInfo fileInfo(fileName);
    QString fileExtension = fileInfo.suffix().toLower();
    QString iconPath;

    // Determine the icon based on the file extension
    if (fileExtension == "jpg" || fileExtension == "jpeg" || fileExtension == "png") {
        iconPath = ":/image.png";
    } else if (fileExtension == "pdf") {
        iconPath = ":/pdf.png";
    } else if (fileExtension == "txt") {
        iconPath = ":/text.png";
    } else {
        iconPath = ":/unknown.png"; // Fallback icon for unsupported extensions
    }

    // Define the alignment style based on the sender
    QString alignmentStyle = isSentByUser ? "text-align: right;" : "text-align: left;";

    // Construct the HTML block for the message (if provided)
   /* QString messageHtml;
    if (!message.isEmpty()) {
        messageHtml = QString("<div style='%1; margin: 5px;'>%2</div>")
        .arg(alignmentStyle, message);
    }*/

    // Construct the HTML block for the file attachment
    QString fileHtml = QString(
                           "<div style='%1; margin: 5px;'>"
                           "  <div style='text-align: right;'>"
                           "    <img src='%2' width='30' height='30'/>"
                           "  </div>"
                           "  <div style='%1; margin-top: 2px;'>"
                           "    %3"
                           "  </div>"
                           "</div>")
                           .arg(alignmentStyle, iconPath, message);


    // Append the message and file attachment to QTextEdit
   /* if (!messageHtml.isEmpty()) {
        ui->chatHistory->insertHtml(messageHtml);
    }
    ui->chatHistory->insertHtml(fileHtml);
    ui->chatHistory->insertHtml("<br>");*/
    ui->chatHistory->append(fileHtml);
}


void MainWindow::appendMessage(const QString &message) {
    ui->chatHistory->append(message);
}

QString MainWindow::formatMessage(const QString &sender, const QString &message) {
    return sender + ": " + message;
}

void MainWindow::on_sendMsgButton_clicked()
{
    ui->attachFile->show();
    QString writeData;
    // Check if a file is attached
    if (!selectedFilePath.isEmpty()) {
        // Send the file to all recipients
        if (ui->recipeintType->currentText() == "Everyone") {
            foreach (QTcpSocket *socket, clientConnectionList) {
                sendFile(socket, selectedFilePath);
            }
        } else if (ui->recipeintType->currentText() == "Private") {
            QString receiverId = ui->clientsList->currentText();
            foreach (QTcpSocket *socket, clientConnectionList) {
                if (socket->socketDescriptor() == receiverId.toLongLong()) {
                    sendFile(socket, selectedFilePath);
                }
            }
        }

        // Clear the attachment after sending
        QFileInfo fileInfo(selectedFilePath);
        QString fileName = fileInfo.fileName();
        writeData=ui->msgData->toPlainText();
        QString fileData=ui->fileHold->toPlainText();
        appendMessage(fileData,fileName,true);
        if(!writeData.isEmpty()){
        appendMessage(writeData,true); // Indicate the file has been sent
        }
        selectedFilePath.clear(); // Reset the file path
    }
    else{

    if (!TCPSocket || !TCPSocket->isOpen()) {
        QMessageBox::information(this, "Error", TCPSocket ? TCPSocket->errorString() : "Socket not initialized.");
        return;
    }

    writeData =ui->msgData->toPlainText();  // Use toPlainText() for QTextEdit
    TCPSocket->write(writeData.toUtf8());
    appendMessage(writeData, true);

    QString messageForClient = ui->msgData->toPlainText();  // Use toPlainText() for QTextEdit
    QString receiver = ui->clientsList->currentText();

    if (ui->recipeintType->currentText() == "Everyone") {
        for (QTcpSocket *socket : clientConnectionList) {
            socket->write(messageForClient.toUtf8());
        }
    } else if (ui->recipeintType->currentText() == "Private") {
        for (QTcpSocket *socket : clientConnectionList) {
            if (socket->socketDescriptor() == receiver.toLongLong()) {
                socket->write(messageForClient.toUtf8());
                break;
            }
        }
    }
    }
    ui->fileHold->clear();
    ui->msgData->clear();  // Use clear() for QTextEdit
}


void MainWindow::readDataFromServer()
{
    if (TCPSocket && TCPSocket->isOpen()) {
        QByteArray Data_From_Server = TCPSocket->readAll();
        QString serverMsg = QString::fromUtf8(Data_From_Server);
        appendMessage(serverMsg,false);
    }
}

void MainWindow::readTextSocket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray messageFromServer = socket->readAll();
    QString message = "Client " + QString::number(socket->socketDescriptor()) + " :: " + QString::fromUtf8(messageFromServer);
    appendMessage(message,false);
}

void MainWindow::readFileSocket()
{
    QTcpSocket *socket=reinterpret_cast<QTcpSocket*>(sender());

    QByteArray dataBuffer;

    QDataStream socketStream;
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream>>dataBuffer;
    if(socketStream.commitTransaction()==false){
        return;
    }
    //get file meta data
    QString headerData=dataBuffer.mid(0,128);     //get header data
    QString fileName=headerData.split(",")[0].split(":")[1];  //file full name
    QString fileExt=fileName.split(":")[1]; //file extension
    QString fileSize=headerData.split(",")[1].split(":")[1]; //get file size

    dataBuffer=dataBuffer.mid(128);  //get file data
    QString saveFilePath=QCoreApplication::applicationDirPath()+"/"+fileName;
    QFile file(saveFilePath);
    if(file.open(QIODevice::WriteOnly)){
        file.write(dataBuffer);
        file.close();
    }
}

void MainWindow::newConnection()
{
    while (TCPServer->hasPendingConnections()) {
        QTcpSocket *newClient = TCPServer->nextPendingConnection();
        connect(newClient, &QTcpSocket::readyRead, this, &MainWindow::readTextSocket);
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

void MainWindow::sendFile(QTcpSocket *socket, QString fileName)
{
    if(socket){
        if(socket->isOpen()){
            QFile fileData(fileName);
            if(fileData.open(QIODevice::ReadOnly)){
                QFileInfo fileInfo(fileData);
                QString fileNameExt(fileInfo.fileName());

                QDataStream socketStream(socket);
                socketStream.setVersion(QDataStream::Qt_5_15);

                QByteArray header;
                header.prepend(("filename:"+fileNameExt.toUtf8()+",filesize:"+QString::number(fileData.size()).toUtf8()));
                header.resize(128);

                QByteArray byteFileData=fileData.readAll();
                byteFileData.prepend(header);

                socketStream<<byteFileData;
            }
            else{
                qDebug()<<"File couldn't be opened!";
            }
        }
        else{
            qDebug()<<"Client socket couldn't be opened!";
        }
    }
    else{
        qDebug()<<"Client socket invalid!";
    }
}

void MainWindow::on_attachFile_clicked()
{
    ui->attachFile->hide();
    // Browse for a file
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", QCoreApplication::applicationDirPath(), "File (*.jpg *.png *.txt *.doc *.pdf)");

    // Check if the user selected a file
    if (!filePath.isEmpty()) {
        selectedFilePath = filePath;

        // Extract file name
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();
        //map file to an icon
        QString fileExtension = fileInfo.suffix().toLower();
        QString iconPath;
        if (fileExtension == "jpg" || fileExtension == "jpeg" || fileExtension == "png") {
            iconPath = ":/image.png";
        } else if (fileExtension == "pdf") {
            iconPath = ":/pdf.png";
        } else if (fileExtension == "txt") {
            iconPath = ":/text.png";
        }
        // HTML to display the icon and text in the message box
        QString html = QString(
                           "<table>"
                           "  <tr>"
                           "    <td><img src='%1' width='25' height='25'></td>"
                           "    <td style='vertical-align:middle;'>%2</td>"
                           "  </tr>"
                           "</table>"
                           ).arg(iconPath, fileName);
        // Display the attachment in the message box
        ui->fileHold->insertHtml(html);
        //ui->msgData->insertHtml("<br>");
        //for scrolling to bottom
       /* QTextCursor cursor = ui->msgData->textCursor();
        cursor.movePosition(QTextCursor::NextRow);
        ui->msgData->setTextCursor(cursor);*/
    }
    else{
        ui->attachFile->show();
    }
}



