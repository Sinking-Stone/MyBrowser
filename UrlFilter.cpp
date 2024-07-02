#include "UrlFilter.h"
#include <QRegularExpression>

UrlFilter::UrlFilter(QObject* parent): QObject(parent)
{

}

QUrl UrlFilter::handleUrlRequest(const QUrl& url){
    QString urlString=url.toString();

    // 本地文件
    if(url.isLocalFile()){
        urlString.replace("qrc:/QML/","");
        return QUrl(urlString);
    }
    // 文本链接
    // https://www.baidu.com/s?wd=
    if(urlString.contains("text:")){
        QString searchText=urlString.mid(urlString.indexOf("text:")+5);
        QString searchUrl="https://www.baidu.com/s?wd="+searchText;
        return QUrl(searchUrl);
    }
    // 淘宝、京东等屏蔽搜索
    QRegularExpression regex("(taobao\\.com|jd\\.com)");
    if(regex.match(urlString).hasMatch()){
        return QUrl("qrc:/QML/errorInfo.html");
    }
    // 如果这是合法的http/https请求
    if(url.isValid()&&(url.scheme()=="http"||url.scheme()=="https")){
        return QUrl(urlString);
    }
    // 如果这里没有协议则添加
    if(!url.scheme().isEmpty()){
        urlString.replace("qrc:/QML/","");
        urlString.insert(0,"https://");
        return QUrl(urlString);
    }
    return QUrl("qrc:/QML/errorInfo.html");
}
