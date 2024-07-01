#ifndef BOOKMARK_H
#define BOOKMARK_H

#include "sqlitetool.h"

// 数据库的字段设计
/**
 * bookMarkID primary key
 * userID     foreign key
 * title
 * url
 * createtime
 */

class BookMark
{
public:
    BookMark();
    ~BookMark();

    bool InsertOneRecord(int userID, const QString& title, const QString& url);

    QVariantList SelectBookMarks(int userid);
    bool RemoveOneBookMark(int userID,const QString& url);

private:
    SqliteTool m_sqlTool;
};

#endif // BOOKMARK_H
