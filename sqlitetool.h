#ifndef SQLITETOOL_H
#define SQLITETOOL_H

#include <QtSql>
#include <QDebug>
#include "DataStorage.h"

class SqliteTool : public DataStorage
{
public:
    SqliteTool(const QString& dbName="myBrowser2024.db");

    virtual void ReadData();
    virtual bool SaveData(const QMap<QString, QString> &data);

    virtual ~SqliteTool();

    bool open();
    void close();
    bool execute(const QString &sql);
    bool initializeDatabases();

    // 书签操作
    QVariantList selectBookMarks(int userid);
    bool insertOneBookMark(int userID, const QString& title, const QString& url);
    bool removeOneBookMark(int userID, const QString& url);

    // 用户表操作
    bool registerUser(const QString& username,const QString& password,const QString& salt,const QString& email,const QString& imagePath);
    QString loginUser(const QString& username,const QString& password);
    QByteArray getUserLogo(const QString& username);
    int getUserId(const QString& username);
    QString getUserSalt(const QString& username);

    // 历史表
    QVariantList selectHistory();
    bool insertOneHistory(const QString& title, const QString& url);
    bool removeOneHistory(int historyID);

private:
    bool createTables();
private:
    QSqlDatabase m_db;
    QString m_dbName;
};

#endif // SQLITETOOL_H
