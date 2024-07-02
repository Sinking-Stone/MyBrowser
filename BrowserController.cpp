#include "BrowserController.h"

BrowserController::BrowserController(QObject* parent) :
    QObject(parent),m_bookMarkStr(""),m_userLoginStr("test"),m_historyStr(""),m_isRecord(true){
    m_user=new User(this);
    m_bookMark=new BookMark();
    m_history=new History(this);
    m_urlFilter=new UrlFilter(this);
}

BrowserController::~BrowserController(){
    delete m_user;
    delete m_bookMark;
    delete m_history;
    delete m_urlFilter;

    for(const auto it:m_pluginInterface)
        delete it;
}

bool BrowserController::registerUser(const QString& username,const QString& password,const QString& email,const QString& imagePath){
    return m_user->registerUser(username,password,email,imagePath);
}
