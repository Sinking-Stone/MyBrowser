#ifndef ADBLOCKINTERCEPTOR_H
#define ADBLOCKINTERCEPTOR_H

#include <QWebEngineUrlRequestInterceptor>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include "logger.h"

class AdBlockInterceptor: public QWebEngineUrlRequestInterceptor{
public:
    AdBlockInterceptor(){}
    virtual void interceptRequest(QWebEngineUrlRequestInfo &info) override{
        QUrl url=info.requestUrl();

        if(m_adPatterns.size()==0){
            if(LoadEasyList()==false) return;
        }
        if(isAdUrl(url)){
            info.block(true);   // 真正屏蔽广告的操作
        }
    }
    void clearPatterns(){
        m_adPatterns.clear();
    }
private:
    bool LoadEasyList(){
        QFile file("./easylist.txt");
        if(file.open((QIODevice::ReadOnly|QIODevice::Text))){
            QTextStream in(&file);
            QString line;
            while (in.readLineInto(&line)) {
                if(!line.startsWith("!")&&!line.isEmpty())
                    m_adPatterns.append(line);
            }
            file.close();
            return true;
        }
        Logger::GetInstance().LogInfo("Open easylist.txt Fail.");
        return false;
    }
    bool isAdUrl(const QUrl& url){
        QString urlString=url.toString();
        for(const QString & keyW:m_adPatterns){
            QRegularExpression regex(keyW);
            QRegularExpressionMatch match=regex.match(urlString);
            if(match.hasMatch()){
                Logger::GetInstance().LogInfo("Found ads: "+urlString);
                return true;
            }
        }
        return false;
    }
    QStringList m_adPatterns;
};

#endif // ADBLOCKINTERCEPTOR_H
