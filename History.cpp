#include "History.h"

History::History(QObject* parent) : QObject(parent)
{
    m_sqlTool.initializeDatabases();
}
History::~History(){

}

bool History::InsertOneHistory(const QString& title, const QString& url){
    return m_sqlTool.insertOneHistory(title,url);
}

QVariantList History::SelectHistory(){
    return m_sqlTool.selectHistory();
}
bool History::RemoveOneHistory(int historyId){
    return m_sqlTool.removeOneHistory(historyId);
}
