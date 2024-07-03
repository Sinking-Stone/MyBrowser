#ifndef BROWSERCONTROLLER_H
#define BROWSERCONTROLLER_H

#include <QObject>
#include <QWebEngineDownloadItem>
#include <QWebEngineView>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtWebEngine>
#include "BookMark.h"
#include "User.h"
#include "BrowserPluginInterface.h"
#include "History.h"
#include "UrlFilter.h"
#include "LanguageTranslator.h"
#include "AdBlockInterceptor.h"
#include "logger.h"
#include "setting.h"

#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QWebEngineView>

class BrowserController : public QObject
{
    Q_OBJECT
    // Q_PROPERTY的说明参考连接：https://blog.csdn.net/m0_74091159/article/details/139039490
    // 用于声明一个类的属性，并将其与类中的成员变量或成员函数关联起来。这使得属性可以通过Qt元对象系统进行访问和操作。
    Q_PROPERTY(bool isRecord READ isRecord WRITE setisRecord NOTIFY isRecordChanged)        // 信号和槽的集成，记录，表示正常浏览器，不记录表示无痕浏览器
    Q_PROPERTY(bool isPreventAds READ isPreventAds WRITE setisPreventAds NOTIFY isPreventAdsChanged)
    Q_PROPERTY(QString bookMark READ bookMark WRITE setbookMark NOTIFY bookMarkChanged)     // 信号和槽的集成，书签
    Q_PROPERTY(QString userLogin READ userLogin WRITE setuserLogin NOTIFY userLoginChanged) // 信号和槽的集成，用户登录
    Q_PROPERTY(QString history READ history WRITE sethistory NOTIFY historyChanged)         // 信号和槽的集成，历史记录

public:
    explicit BrowserController(QGuiApplication& app, QQmlApplicationEngine& engine,QObject* parent=nullptr);
    ~BrowserController();

    // 加载插件
    void loadPlugins(){
        QDir pluginsDir("./plugins/");
        const auto entryList=pluginsDir.entryList(QDir::Files);
        for(const QString& fileName:entryList){
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));  // 取绝对路径
            QObject* plugin=pluginLoader.instance();
            if(plugin){
                BrowserPluginInterface *pluginInterface=qobject_cast<BrowserPluginInterface*>(plugin);
                if(pluginInterface)
                    m_pluginInterface.push_back(pluginInterface);
            }
        }
    }

    int isRecord() const{
        return m_isRecord;
    }
    void setisRecord(bool isRecord){
        if(m_isRecord!=isRecord){
            m_isRecord=isRecord;
            emit isRecordChanged();
        }
    }

    // 屏蔽广告
    int isPreventAds() const{return m_isPreventAds;}
    void setisPreventAds(bool isPreventAds){
        if(m_isPreventAds!=isPreventAds){
            m_isPreventAds=isPreventAds;
            if(m_isPreventAds==true){
                m_pWebProfile->setRequestInterceptor(m_adBlock);
                Setting::GetInstance().SetValue("PreventAds","1");
                Logger::GetInstance().LogInfo("set PreventAds True");
            }else{
                m_pWebProfile->setRequestInterceptor(nullptr);
                m_adBlock->clearPatterns();
                Setting::GetInstance().SetValue("PreventAds","0");
                Logger::GetInstance().LogInfo("set PreventAds False");
            }
            emit isPreventAdsChanged();
        }

    }

    // 截图插件的操作
    Q_INVOKABLE QString doSnapshot(const QString& info){
        if(m_pluginInterface.size()==0) // 如果组件没有被加载
            loadPlugins();
        for(auto &it:m_pluginInterface){
            if(it->isKindOf("snapshot"))    // 插件是截图
                return it->performAction(nullptr,info);
        }
        return "snapshot over!!!";
    }

    // history
    Q_INVOKABLE QVariantList fetchHistory(){
        QVariantList dataList=m_history->SelectHistory();
        return dataList;
    }
    QString history() const {
        return m_historyStr;
    }
    void sethistory(QString historyStr){
        if(m_historyStr!=historyStr){
            m_historyStr=historyStr;
            QStringList sList=historyStr.split(",--,");
            if(m_isRecord==true)
                m_history->InsertOneHistory(sList[0],sList[1]);
            emit historyChanged();
            m_historyStr="";
        }
    }
    Q_INVOKABLE bool removeHistory(int historyID){
        return m_history->RemoveOneHistory(historyID);
    }

    // 查询书签数据
    Q_INVOKABLE QVariantList fetchBookMark(){
        QVariantList dataList=m_bookMark->SelectBookMarks(getUserId(m_userLoginStr));
        return dataList;
    }
    QString bookMark() const{
        return m_bookMarkStr;
    }
    void setbookMark(QString bookMarkStr){
        if(bookMarkStr!=m_bookMarkStr) {
            m_bookMarkStr=bookMarkStr;
            QStringList sList=bookMarkStr.split(",--,");
            m_bookMark->InsertOneRecord(getUserId(m_userLoginStr),sList[0],sList[1]);    // 插入书签的时候要确定是哪一个用户插入的行为
            emit bookMarkChanged();          // 表示一旦数据发生改变时，就会发生myValueChanged信号
            m_bookMarkStr="";
        }
    }
    Q_INVOKABLE bool removeBookMark(int userID,const QString& url){
        return m_bookMark->RemoveOneBookMark(userID,url);
    }

    // 用户登录
    QString userLogin() const{ return m_userLoginStr;}
    void setuserLogin(QString userLoginStr){
        if(userLoginStr!=m_userLoginStr){
            m_userLoginStr=userLoginStr;
            emit userLoginChanged();
        }
    }

public slots:
    bool registerUser(const QString& username,const QString& password,const QString& email,const QString& imagePath);
    bool loginUser(const QString& username,const QString& password){
        setuserLogin(m_user->loginUser(username,password));
        return true;
    }
    QString getUserLogo(const QString& username){
        QByteArray logoData=m_user->getUserLogo(username);   // 获取图像数据是二进制流信息
        return QString("data:/image/png;base64,")+logoData.toBase64();  // 返回64未string为了适配前端
    }
    int getUserId(const QString& username){
        return m_user->getUserId(username);
    }

    QUrl handleUrlRequest(const QUrl& url){
        qDebug()<<url;
        QUrl ans=m_urlFilter->handleUrlRequest(url);
        qDebug()<<ans;
        return ans;
    }

    int getLanguage() const{
        return m_translator->getLanguage();
    }
    bool setLanguage(int indexOfLanguage){
        return m_translator->setLanguage(indexOfLanguage);
    }

    // webengine prfile
    void processWebEngineProfile(QWebEngineProfile* profile){
        QObject *webEngineProfileObj=nullptr;
        for(QObject* rootObject : m_translator->getQmlEngine()->rootObjects()){
            webEngineProfileObj=findObjectByName(rootObject,"WebEngineP");
            if(webEngineProfileObj){
                qDebug()<<"QWebEngineProfile object found in QML";
                Logger::GetInstance().LogInfo("QWebEngineProfile object found in QML");
                break;
            }
        }
        if(!webEngineProfileObj){
            qDebug()<<"QWebEngineProfile object not found in QML";
            Logger::GetInstance().LogInfo("QWebEngineProfile object not found in QML");
            return;
        }
        m_pWebProfile=((QWebEngineProfile*)webEngineProfileObj);
    }
private:
    QObject* findObjectByName(QObject* parent,const QString &name){
        if(parent->objectName()==name)
            return parent;
        for(QObject* child:parent->children()){
            QObject* foundObj=findObjectByName(child,name);
            if(foundObj) return foundObj;
        }
        return nullptr;
    }

    void printCookies(QWebEngineProfile* profile){
        QWebEngineCookieStore* cookieStore=profile->cookieStore();
        QObject::connect(cookieStore,&QWebEngineCookieStore::cookieAdded,[cookieStore](const QNetworkCookie& cookie){
            qDebug()<<"Added cookie:";
            qDebug()<<"name: "<<cookie.name();
            qDebug()<<"value: "<<cookie.value();
            qDebug()<<"domain: "<<cookie.domain();
            qDebug()<<"path: "<<cookie.path();
            qDebug()<<"expiration: "<<cookie.expirationDate();
        });
        QObject::connect(cookieStore,&QWebEngineCookieStore::cookieRemoved,[](const QNetworkCookie& cookie){
            qDebug()<<"Removed cookie:";
            qDebug()<<"name: "<<cookie.name();
            qDebug()<<"value: "<<cookie.value();
            qDebug()<<"domain: "<<cookie.domain();
            qDebug()<<"path: "<<cookie.path();
            qDebug()<<"expiration: "<<cookie.expirationDate();
        });
    }
signals:
    void bookMarkChanged();
    void userLoginChanged();
    void historyChanged();
    void isRecordChanged();
    void isPreventAdsChanged();

private:
    // 书签
    QString m_bookMarkStr;
    BookMark* m_bookMark;

    // 用户
    QString m_userLoginStr;
    User* m_user;

    // 历史
    QString m_historyStr;
    History* m_history;
    // 是否无痕浏览
    bool m_isRecord;

    // 浏览器配置的profile
    QWebEngineProfile* m_pWebProfile;
    // 广告
    bool m_isPreventAds;
    AdBlockInterceptor* m_adBlock;

    // 地址网页浏览
    UrlFilter* m_urlFilter;
    // 语言翻译
    LanguageTranslator* m_translator;
    // 插件
    QVector<BrowserPluginInterface*> m_pluginInterface;
};

#endif // BROWSERCONTROLLER_H
