#include "session.h"
#include <QCryptographicHash>

Session::~Session()
{
    emit startLogout(m_userId);
}

int Session::getDrgId() const
{
    return drgId;
}

void Session::loginResult(int userId)
{
    m_userId = userId;
    emit loginReady(userId);
}

Session::Session(Repository *db, QObject *parent):
    QObject(parent),
    m_db(db)
{
    m_userId = -1;
    connect(this, &Session::login, db, &Repository::login);
    connect(db, &Repository::loginReady, this, &Session::loginResult);
    connect(this, &Session::startLogout, db, &Repository::logout);

}

void Session::authenticate(const QString &user, const QString &pw)
{
    emit login(user, pw);
}

QString Session::encrypt(const QByteArray &pw)
{
    return QCryptographicHash::hash(pw, QCryptographicHash::Sha512);
}
