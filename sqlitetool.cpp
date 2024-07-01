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

bool SqliteTool::initializeDatabases(){
    if(!open()){
        return false;
    }
    if(!createTables()){
        qDebug()<<"Failed to create tables.";
        return false;
    }
    return true;
}

/**
 * Bookmarks
 * bookMarkID primary key
 * userID     foreign key
 * title
 * url
 * createtime
 */
// 创建书签表
bool SqliteTool::createTables(){
    QString sql=QString("CREATE TABLE IF NOT EXISTS Users ("
                        "userID INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "userName TEXT UNIQUE NOT NULL,"
                        "password  TEXT NOT NULL,"
                        "salt TEXT NOT NULL,"
                        "email TEXT UNIQUE,"
                        "logo BLOB,"
                        "createTime DATETIME DEFAULT CURRENT_TIMESTAMP"
                        ")");

    if(!execute(sql)){
        qDebug()<<"create table Users fail!!!";
        return false;
    }
    sql=QString("CREATE TABLE IF NOT EXISTS Bookmarks ("
                       "bookMarkID INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "userID INTEGER,"
                       "title TEXT NOT NULL,"
                       "url TEXT NOT NULL,"
                       "createTime DATETIME DEFAULT CURRENT_TIMESTAMP,"
                       "FOREIGN KEY(UserID) REFERENCES Users (userID)"
                       ")");
    if(!execute(sql)){
        qDebug()<<"create table Bookmarks fail!!!";
        return false;
    }

    sql=QString("CREATE TABLE IF NOT EXISTS History ("
                "historyID INTEGER PRIMARY KEY AUTOINCREMENT,"
                "title TEXT NOT NULL,"
                "url TEXT NOT NULL,"
                "createTime DATETIME DEFAULT CURRENT_TIMESTAMP"
                ")");
    if(!execute(sql)){
        qDebug()<<"create table history fail!!!";
        return false;
    }

   return true;
}// url唯一是为了防止添加重复书签

// 根据用户获取所有的书签
QVariantList SqliteTool::selectBookMarks(int userid){
    QVariantList resultList;
    QSqlQuery query;
    query.prepare("SELECT * FROM Bookmarks WHERE userID=?");
    query.addBindValue(userid);
    if(query.exec()){
        while (query.next()) {
            QSqlRecord record=query.record();
            QMap<QString,QVariant> row;
            for(int i=0;i<record.count();++i){
                row[record.fieldName(i)]=record.value(i);
            }
            resultList.append(row);
        }
    }
    return resultList;
}

bool SqliteTool::insertOneBookMark(int userID, const QString& title, const QString& url){
    QSqlQuery query;

    query.prepare("SELECT 1 FROM Bookmarks WHERE UserID=? AND title = ? AND url = ?");
    query.addBindValue(userID);
    query.addBindValue(title);
    query.addBindValue(url);
    if(!query.exec()){
        qDebug()<<"execute error";
        return false;
    }
    if(query.next()){
        qDebug()<<"the record is exist";
        return  false;
    }
    query.prepare("INSERT INTO Bookmarks (userID, title, url) VALUES(?,?,?)");
    query.addBindValue(userID);
    query.addBindValue(title);
    query.addBindValue(url);

    return  query.exec();
}

bool SqliteTool::removeOneBookMark(int userID, const QString& url){
    QSqlQuery query;
    query.prepare("DELETE FROM Bookmarks WHERE userID=? AND url=?");
    query.addBindValue(userID);
    query.addBindValue(url);

    return query.exec();
}

// 读取图像的二进制数据，没有在头文件中定义，无需加类域
QByteArray readImageFile(const QString& imagePath){
    QFile file(imagePath);
    if(!file.open(QIODevice::ReadOnly))
        return QByteArray();
    return file.readAll();
}

// 用户注册操作
bool SqliteTool::registerUser(const QString& username,const QString& password,const QString& salt,const QString& email,const QString& imagePath){
    QString cleanPath=imagePath;
    // 对windows操作系统要删除file:///，其他操作系统删除file://
#ifdef Q_OS_WIN
    cleanPath.remove("file:///");
#else
    cleanPath.remove("file://");
#endif
    QByteArray imageData=readImageFile(cleanPath);

    QSqlQuery query;
    query.prepare("INSERT INTO Users (userName, password, salt, email, logo) VALUES(?,?,?,?,?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(salt);
    query.addBindValue(email);
    query.addBindValue(imageData);

    return query.exec();
}

QString SqliteTool::loginUser(const QString& username,const QString& password){
    QSqlQuery query;
    query.prepare("SELECT userName FROM Users WHERE userName=? AND password=?");
    query.addBindValue(username);
    query.addBindValue(password);

    if(query.exec()&&query.next()){
        return username;
    }
    qDebug()<<"login error:"<<query.lastError();
    return QString();
}

// 由于Logo是一个二进制流信息，所以返回的时候给出QByteArray类型
QByteArray SqliteTool::getUserLogo(const QString& username){
    QSqlQuery query;
    query.prepare("SELECT logo FROM Users WHERE userName=? ");
    query.addBindValue(username);

    if(query.exec()&&query.next()){
        return query.value(0).toByteArray();
    }
    return QByteArray();
}

int SqliteTool::getUserId(const QString& username){
    QSqlQuery query;
    query.prepare("SELECT userID FROM Users WHERE userName=? ");
    query.addBindValue(username);

    if(query.exec()&&query.next()){
        return query.value(0).toInt();
    }
    return -1;
}

QString SqliteTool::getUserSalt(const QString& username){
    QSqlQuery query;
    query.prepare("SELECT salt FROM Users WHERE userName=? ");
    query.addBindValue(username);
    if(query.exec()&&query.next()){
        return query.value(0).toString();
    }
    return QString();
}

QVariantList SqliteTool::selectHistory(){
    QVariantList resultList;
    QSqlQuery query;
    query.prepare("SELECT * FROM History");
    if(query.next()){
        while (query.next()) {
            QSqlRecord record=query.record();
            QMap<QString,QVariant> row;
            for(int i=0;i<record.count();++i)
                row[record.fieldName(i)]=record.value(i);
            resultList.append(row);
        }
    }
    return resultList;
}

bool SqliteTool::insertOneHistory(const QString& title, const QString& url){
    QSqlQuery query;
    query.prepare("INSERT INTO History (title,url) VALUES(?,?)");
    query.addBindValue(title);
    query.addBindValue(url);

    return query.exec();
}

bool SqliteTool::removeOneHistory(int historyID){
    QSqlQuery query;
    query.prepare("DELETE FROM History WHERE historyID=?");
    query.addBindValue(historyID);

    return query.exec();
}

void SqliteTool::ReadData(){
    // 查询所有记录
//    qDebug()<<"所有记录: ";
//    QSqlQuery query=select("mySpyder2024");
//    while(query.next()){
//        QString title=query.value(0).toString();
//        QString url=query.value(1).toString();

//        qDebug()<<"title: "<<title<<", url: "<<url;
//    }
//    qDebug()<<"end...";
}
bool SqliteTool::SaveData(const QMap<QString, QString> &data){
    //打开数据库
    if(!open()) return false;
    // 创建名为Users的表
//    QString fields=" title TEXT ,url TEXT PRIMARY KEY";
//    if(!createTable("mySpyder2024",fields))
//        return false;
//    for(auto it=data.begin();it!=data.end();++it){
//        QStringList fieldsList={"title","url"};
//        QStringList valuesList={it.key(),it.value()};
//        if(!insert("mySpyder2024",fieldsList,valuesList))
//            qDebug()<<"Insert Failed "<<fieldsList<<" "<<valuesList;
//    }
    return true;
}

SqliteTool::~SqliteTool(){
    m_db.close();
}
