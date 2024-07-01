#ifndef USER_H
#define USER_H

#include <QObject>
#include "sqlitetool.h"

class User:public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent=nullptr);
    ~User();

    bool registerUser(const QString& username,const QString& password,const QString& email,const QString& imagePath);
    QString loginUser(const QString& username,const QString& password);
    QByteArray getUserLogo(const QString& username);
    int getUserId(const QString& username);
    QString getUserSalt(const QString& username);


private:
    SqliteTool m_sqlTool;
};

#endif // USER_H
