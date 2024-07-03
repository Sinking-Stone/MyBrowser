#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>
#include "BrowserController.h"
#include <QQmlContext>
#include <QWebEngineProfile>

static QUrl startupUrl(){
    return QUrl(QStringLiteral("https://www.baidu.com/"));          // 先固定网页
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);    // 这是为了对不同硬件来适配的设置
    QCoreApplication::setOrganizationName("MyBrowser");             // 设置组织的名称

    QGuiApplication app(argc, argv);                                // 程序的入口
    QQmlApplicationEngine engine;                                   // 这是启动QML的引擎

    // mycode add
    QtWebEngine::initialize();                                                  // 初始化web浏览器引擎，采用chromium内核
    BrowserController browserController(app,engine);                                        // 创建QQmlContext类的目的是为了将browserController对象放在qml对象上，传递数据
    QQmlContext* qmlContext=engine.rootContext();                               // 获取根节点
    qmlContext->setContextProperty("browserController",&browserController); // 将对象传入到qml页面上，第一个参数是对象名，第二个参数是对象地址
    // mycode add

    const QUrl url(QStringLiteral("qrc:/QML/main.qml"));
    engine.load(url);

    // mycode add
    // load qml and create window
    QMetaObject::invokeMethod(engine.rootObjects().first(),"load",Q_ARG(QVariant,startupUrl()));    // 当页面加载完之后向第一个元素的load方法传入startupUrl函数

    return app.exec();
}
