#include "sqlitetool.h"
#include <QtSql/QSqlError>

SqliteTool::SqliteTool(const QString& dbName):m_dbName(dbName) {
    // 添加SQLite 驱动
    m_db=QSqlDatabase::addDatabase("QSQLITE");
    // 设置数据库文件名
    m_db.setDatabaseName(m_dbName);
}

bool SqliteTool::open(){
    if(!m_db.open()){
        qDebug()<<"Failed to open database: "<<m_db.lastError().text();
        return false;
    }
    return true;
}

void SqliteTool::close(){
    m_db.close();
}

bool SqliteTool::execute(const QString &sql){
    QSqlQuery query;
    if(!query.exec(sql)){
        qDebug()<<"Failed to open database: "<<query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery SqliteTool::executeQuery(const QString &sql){
    QSqlQuery query;
    if(!query.exec(sql)){
        qDebug()<<"Failed to open database: "<<query.lastError().text();
    }
    return query;
}

bool SqliteTool::createTable(const QString &tableName,const QString &fields){
   QString sql=QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(tableName,fields);
   return execute(sql);
}

bool SqliteTool::insert(const QString &tableName,const QStringList &fields,const QStringList &values){
    QString sql=QString("INSERT INTO %1 (%2) VALUES('%3')").arg(tableName,fields.join("', '"),values.join("', '"));
    return execute(sql);
}

bool SqliteTool::update(const QString &tableName,const QString &update,const QString& condition){
    QString sql=QString("UPDATE %1 SET %2").arg(tableName,update);
    if(!condition.isEmpty()){
        sql+=QString(" WHERE %1").arg(condition);
    }
    return execute(sql);
}

bool SqliteTool::remove(const QString &tableName,const QString &condition){
    QString sql=QString("DELETE FROM %1 WHERE %2").arg(tableName,condition);
    return execute(sql);
}

QSqlQuery SqliteTool::select(const QString &tableName,const QStringList &fields,const QString& condition){
    QString sql;
    if(fields.isEmpty()){
        sql=QString("SELECT * FROM %1").arg(tableName);
    }else{
        sql=QString("SELECT %1 FROM %1").arg(fields.join(", ") ,tableName);
    }
    if(!condition.isEmpty()){
        sql+=QString(" WHERE %1").arg(condition);
    }
    return executeQuery(sql);
}

void SqliteTool::ReadData(){
    // 查询所有记录
    qDebug()<<"所有记录: ";
    QSqlQuery query=select("mySpyder2024");
    while(query.next()){
        QString title=query.value(0).toString();
        QString url=query.value(1).toString();

        qDebug()<<"title: "<<title<<", url: "<<url;
    }
    qDebug()<<"end...";
}
bool SqliteTool::SaveData(const QMap<QString, QString> &data){
    //打开数据库
    if(!open()) return false;
    // 创建名为Users的表
    QString fields=" title TEXT ,url TEXT PRIMARY KEY";
    if(!createTable("mySpyder2024",fields))
        return false;
    for(auto it=data.begin();it!=data.end();++it){
        QStringList fieldsList={"title","url"};
        QStringList valuesList={it.key(),it.value()};
        if(!insert("mySpyder2024",fieldsList,valuesList))
            qDebug()<<"Insert Failed "<<fieldsList<<" "<<valuesList;
    }
    return true;
}

SqliteTool::~SqliteTool(){
    m_db.close();
}
