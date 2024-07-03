#include "BrowserController.h"
#include "setting.h"

BrowserController::BrowserController(QGuiApplication& app, QQmlApplicationEngine& engine,QObject* parent) :
    QObject(parent),m_bookMarkStr(""),m_userLoginStr("test"),m_historyStr(""),m_isRecord(true){
    m_user=new User(this);
    m_bookMark=new BookMark();
    m_history=new History(this);
    m_urlFilter=new UrlFilter(this);
    m_translator = new LanguageTranslator(app, engine);
    m_adBlock=new AdBlockInterceptor();

    m_isPreventAds=false;
    QVariant isPreventAds=Setting::GetInstance().GetValue("PreventAds");
    if(isPreventAds.isValid()){
        if(isPreventAds.toInt()==1)
            m_isPreventAds=true;
    }
}

BrowserController::~BrowserController(){
    delete m_user;
    delete m_bookMark;
    delete m_history;
    delete m_urlFilter;
    delete m_translator;
    delete m_adBlock;

    for(const auto it:m_pluginInterface)
        delete it;
    Setting::GetInstance().SetValue("HomePage","https://www.baidu.com");
    Setting::GetInstance().SyncSetting();
}

bool BrowserController::registerUser(const QString& username,const QString& password,const QString& email,const QString& imagePath){
    return m_user->registerUser(username,password,email,imagePath);
}
