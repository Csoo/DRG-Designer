#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT

    int m_id;
    QString m_userName;
    bool isFirstLogin;
public:
    explicit User(QObject *parent = nullptr);

    Q_INVOKABLE bool authenticate(const QString &user, const QString &pw);
    Q_INVOKABLE QString encrypt(const QByteArray &pw);
signals:

};

#endif // USER_H
