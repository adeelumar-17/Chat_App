#include "hashutil.h"
const QString HashUtil::globalSalt = "S3cureS@lt2024!";
HashUtil::HashUtil() {}
QString HashUtil::hashPassword(const QString &password) {
    QByteArray saltedPassword = password.toUtf8() + HashUtil::globalSalt.toUtf8();
    QByteArray hash = QCryptographicHash::hash(saltedPassword, QCryptographicHash::Sha256);
    return QString::fromUtf8(hash.toHex());
}
