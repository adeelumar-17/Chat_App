#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>    //add necessary network libraries
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QList>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QDataStream>
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
    void readDataFromSocket();
    void newConnection();

private:
    Ui::MainWindow *ui;
    QString currentUser;
    QTcpSocket *TCPSocket; //tcp socket for reliable connections
    QTcpServer *TCPServer; //for server functionality
    QList<QTcpSocket*> clientConnectionList;
    void setupDatabase();
    QString formatMessage(const QString &sender, const QString &message);
    void appendMessage(const QString &message);
    void addNewClientConnection(QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
};

#endif // MAINWINDOW_H
