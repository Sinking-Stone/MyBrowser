#ifndef SQLITETOOL_H
#define SQLITETOOL_H

#include <QtSql/QSqlQuery>
#include <QDebug>
#include "DataStorage.h"

class SqliteTool : public DataStorage
{
public:
    SqliteTool(const QString& dbName="mySpyder2024.db");

    virtual void ReadData();
    virtual bool SaveData(const QMap<QString, QString> &data);

    virtual ~SqliteTool();

    bool open();
    void close();
    bool execute(const QString &sql);
    QSqlQuery executeQuery(const QString &sql);
    bool createTable(const QString &tableName,const QString &fields);
    bool insert(const QString &tableName,const QStringList &fields,const QStringList &values);
    bool update(const QString &tableName,const QString &update,const QString& condition="");
    bool remove(const QString &tableName,const QString &condition);
    QSqlQuery select(const QString &tableName,const QStringList &fields=QStringList(),const QString& condition="");
private:
    QSqlDatabase m_db;
    QString m_dbName;
};

#endif // SQLITETOOL_H
