#ifndef HASHUTIL_H
#define HASHUTIL_H
#include <QCryptographicHash>
#include <QString>
class SignUP;
class Login;
class HashUtil
{
    friend class SignUp;
    friend class Login;
public:
    HashUtil();

private slots:
    static QString hashPassword(const QString &password);
private:
    static const QString globalSalt;
};

#endif // HASHUTIL_H
