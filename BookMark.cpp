#include "BookMark.h"

BookMark::BookMark()
{
    m_sqlTool.initializeDatabases();
}

BookMark::~BookMark(){

}

bool BookMark::InsertOneRecord(int userID, const QString& title, const QString& url){
    if(!m_sqlTool.insertOneBookMark(userID,title,url))
        return false;
    return true;
}

QVariantList BookMark::SelectBookMarks(int userid){
    return  m_sqlTool.selectBookMarks(userid);
}

bool BookMark::RemoveOneBookMark(int userID,const QString& url){
    return m_sqlTool.removeOneBookMark(userID,url);
}
