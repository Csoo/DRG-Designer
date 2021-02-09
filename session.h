#ifndef SESSION_H
#define SESSION_H

#include "repository.h"

#include <QObject>

class Session : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int drgId READ getDrgId)

    int drgId;
    int m_userId;
    QString m_userName;
    bool isFirstLogin;

    Repository *m_db;
public:
    explicit Session(Repository *db, QObject *parent = nullptr);
    ~Session();

    Q_INVOKABLE void authenticate(const QString &user, const QString &pw);
    Q_INVOKABLE QString encrypt(const QByteArray &pw);
    Q_INVOKABLE void logout() { emit startLogout(m_userId); };
    int getDrgId() const;

signals:
    void login(const QString &userName, const QString &pw);
    void loginReady(int userId);
    void startLogout(int userId);

public slots:
    void loginResult(int userId);
};

#endif // USER_H
