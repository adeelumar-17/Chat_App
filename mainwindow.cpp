#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
void mainWindow::setupDatabase() {
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

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
}

mainWindow::~mainWindow()
{
    delete ui;
}
