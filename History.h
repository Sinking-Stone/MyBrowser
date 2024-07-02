#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include "sqlitetool.h"

class History:public QObject
{
    Q_OBJECT
public:
    explicit History(QObject* parent=nullptr);
    ~History();

    bool InsertOneHistory(const QString& title, const QString& url);
    QVariantList SelectHistory();   // 查询出所有的记录
    bool RemoveOneHistory(int historyId);
private:
    SqliteTool m_sqlTool;
};

#endif // HISTORY_H
