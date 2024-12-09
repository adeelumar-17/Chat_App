#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork> //add necessary network libraries
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QList>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QLayout>
#include "login.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendMsgButton_clicked();
    void readDataFromServer();
    void readTextSocket();
    void readFileSocket(const QByteArray &dataBuffer);
    void newConnection();
    void keyPressEvent(QKeyEvent *event);
    void on_attachFile_clicked();

private:
    Ui::MainWindow *ui;
    QString currentUser;
    QTcpSocket *TCPSocket; //tcp socket for reliable connections
    QTcpServer *TCPServer; //for server functionality
    QString selectedFilePath;
    QList<QTcpSocket*> clientConnectionList;
    void setupDatabase();
    QString formatMessage(const QString &sender, const QString &message);
    void appendMessage(const QString &message);
    void appendMessage(const QString &message, bool isSentByUser);
    void appendMessage(const QString &message,const QString &fileName, bool isSentByUser);
    void addNewClientConnection(QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
    void sendFile(QTcpSocket* socket, QString fileName);
};

#endif // MAINWINDOW_H
