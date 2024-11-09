#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <QDebug>

class Database
{
public:
    Database();  // Constructor to set up the database connection
    bool createUserTable();  // Method to create the user table
    bool insertUser(const QString &username, const QString &email,const QString &password);  // Method to insert user data
    void fetchUsers();  // Method to retrieve user data
    // Declare a method to get user data by username or email
    QSqlQuery getUserByIdentifier(const QString &identifier);


private:
    QSqlDatabase db;
    bool openDatabase();  // Private method to open the database
};

#endif // DATABASE_H
