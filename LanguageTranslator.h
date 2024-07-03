#ifndef LANGUAGETRANSLATOR_H
#define LANGUAGETRANSLATOR_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

class LanguageTranslator : public QObject
{
    Q_OBJECT
public:
    explicit LanguageTranslator(QGuiApplication& app,QQmlApplicationEngine& engine,QObject *parent = nullptr);

    bool setLoaclLanguage();

    Q_INVOKABLE bool setLanguage(int indexOfLanguage);
    Q_INVOKABLE int  getLanguage() const;

    QQmlApplicationEngine* getQmlEngine(){
        return m_engine;
    }
signals:
private:
    QGuiApplication* m_app;
    QQmlApplicationEngine* m_engine;
    QTranslator* m_translator;
};

#endif // LANGUAGETRANSLATOR_H
