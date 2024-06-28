#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QMap>
#include <QString>

// 数据存储接口
class DataStorage{
public:
    virtual bool SaveData(const QMap<QString,QString>&data)=0;
    virtual void ReadData()=0;
};


#endif // DATASTORAGE_H
