#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>    //add necessary network libraries
#include <QtNetwork/QHostAddress>
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

private:
    Ui::MainWindow *ui;
    QString currentUser;
    QTcpSocket *TCPSocket; //for tcp socket for reliable connections
    void setupDatabase();
    void appendMessage(const QString &message);
};

#endif // MAINWINDOW_H
