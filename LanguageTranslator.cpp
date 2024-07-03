#include "LanguageTranslator.h"
#include <QTranslator>
#include <QDebug>

LanguageTranslator::LanguageTranslator(QGuiApplication& app,QQmlApplicationEngine& engine,QObject *parent) : QObject(parent)
{
    this->m_app=&app;
    this->m_engine=&engine;

    m_translator=new QTranslator();
    setLoaclLanguage();

}
bool LanguageTranslator::setLoaclLanguage(){
    QLocale locale;
    m_app->removeTranslator(m_translator);
    if(locale.language()==QLocale::Chinese){
        if(!m_translator->load(":/Browser_zh_CN.qm")){
            qDebug()<<"load translator error!!!";
            return false;
        }
    }else if(locale.language()==QLocale::English){
        if(!m_translator->load(":/Browser_en_US.qm")){
            qDebug()<<"load translator error!!!";
            return false;
        }
    }
    if(!m_app->installTranslator(m_translator)){
        qDebug()<<"install translator error!!!";
        return false;
    }
    m_engine->retranslate();
    return true;
}

bool LanguageTranslator::setLanguage(int indexOfLanguage){
    m_app->removeTranslator(m_translator);
    if(indexOfLanguage==0){
        if(!m_translator->load(":/Browser_zh_CN.qm")){
            qDebug()<<"load translator error!!!";
            return false;
        }
    }else if(indexOfLanguage==1){
        if(!m_translator->load(":/Browser_en_US.qm")){
            qDebug()<<"load translator error!!!";
            return false;
        }
    }
    if(!m_app->installTranslator(m_translator)){
        qDebug()<<"install translator error!!!";
        return false;
    }
    m_engine->retranslate();
    return true;
}
int  LanguageTranslator::getLanguage() const{
    QLocale locale;
    if(locale.language()==QLocale::Chinese){
        return 0;
    }else if(locale.language()==QLocale::English){
        return 1;
    }
    return 1;
}
