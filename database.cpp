#include "database.h"

Database::Database() {
    // Constructor calls the openDatabase function to initialize the DB connection
    if (!openDatabase()) {
        qDebug() << "Database could not be opened!";
    } else {
        qDebug() << "Database opened successfully.";
    }
}

bool Database::openDatabase() {
    // Set the database path in the writable AppData location
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/user_credentials.db";
    QDir().mkpath(QFileInfo(dbPath).path());  // Ensure directory exists

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Error: Unable to open the database!" << db.lastError().text();
        return false;
    }
    return true;
}

bool Database::createUserTable() {
    QSqlQuery query;
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            email TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        )
    )";

    if (!query.exec(createTableQuery)) {
        qDebug() << "Error creating table:" << query.lastError().text();
        return false;
    }
    qDebug() << "User table created successfully.";
    return true;
}

bool Database::insertUser(const QString &username, const QString &email, const QString &password)
{
    QSqlQuery query;
    // Prepare the insert query with placeholders for username, email, and password
    query.prepare("INSERT INTO users (username, email, password) VALUES (?, ?, ?)");

    // Bind the actual values to the placeholders
    query.addBindValue(username);  // Bind username value
    query.addBindValue(email);     // Bind email value (this is the new parameter added)
    query.addBindValue(password);  // Bind password value

    // Execute the query and check for errors
    if (!query.exec()) {
        qDebug() << "Error inserting user: " << query.lastError();
    } else {
        qDebug() << "User inserted successfully!";
    }
    return true;
}


void Database::fetchUsers() {
    QSqlQuery query("SELECT id, username FROM users");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        qDebug() << "User ID:" << id << ", Username:" << username;
    }
}
QSqlQuery Database::getUserByIdentifier(const QString &identifier) {
    QSqlQuery query;
    // Check if identifier contains '@' to distinguish email from username
    if (identifier.contains('@')) {
        query.prepare("SELECT username, email, password FROM users WHERE email = :identifier");
    } else {
        query.prepare("SELECT username, email, password FROM users WHERE username = :identifier");
    }

    query.bindValue(":identifier", identifier);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return query;
}


