#include "user.h"
#include <QCryptographicHash>

User::User(QObject *parent) : QObject(parent)
{
}

bool User::authenticate(const QString &user, const QString &pw)
{

}

QString User::encrypt(const QByteArray &pw)
{
    return QCryptographicHash::hash(pw, QCryptographicHash::Sha512);
}
