#ifndef URLFILTER_H
#define URLFILTER_H

#include <QObject>
#include <QUrl>

class UrlFilter:public QObject
{
    Q_OBJECT
public:
    explicit UrlFilter(QObject* parent=nullptr);

    QUrl handleUrlRequest(const QUrl& url);
};

#endif // URLFILTER_H
