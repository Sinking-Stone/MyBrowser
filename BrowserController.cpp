#include "BrowserController.h"

BrowserController::BrowserController(QObject* parent) : QObject(parent),m_bookMarkStr(""),m_userLoginStr("test"){
}

BrowserController::~BrowserController(){
    delete m_user;
    delete m_bookMark;
    delete m_history;
    delete m_urlFilter;
}

bool BrowserController::registerUser(const QString& username,const QString& password,const QString& email,const QString& imagePath){
    return m_user->registerUser(username,password,email,imagePath);
}
