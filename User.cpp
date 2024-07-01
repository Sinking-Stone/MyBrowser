#include "User.h"
#include <QCryptographicHash>

User::User(QObject *parent):QObject(parent)
{
    m_sqlTool.initializeDatabases();
    registerUser("test","123456","test@test.com","./icons/login.png");
}

User::~User(){}

QString HashPassword(const QString& password,const QString& salt){
    QString combinedPassword=password+salt;
    QByteArray hashResult=QCryptographicHash::hash(combinedPassword.toUtf8(),QCryptographicHash::Sha256);
    return QString(hashResult.toHex());
}

bool User::registerUser(const QString& username,const QString& password,const QString& email,const QString& imagePath){
    QString salt=QString::number(qrand());  // 生成随机盐
    QString hashPassword=HashPassword(password,salt);
    return m_sqlTool.registerUser(username,hashPassword,salt,email,imagePath);
}

QString User::loginUser(const QString& username,const QString& password){
    return m_sqlTool.loginUser(username,HashPassword(password,getUserSalt(username)));
}

QByteArray User::getUserLogo(const QString& username){
    return m_sqlTool.getUserLogo(username);
}

int User::getUserId(const QString& username){
    return m_sqlTool.getUserId(username);
}

QString User::getUserSalt(const QString& username){
    return m_sqlTool.getUserSalt(username);
}
