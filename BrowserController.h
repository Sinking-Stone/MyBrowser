#ifndef BROWSERCONTROLLER_H
#define BROWSERCONTROLLER_H

#include <QObject>

class BrowserController : public QObject
{
    Q_OBJECT
    // Q_PROPERTY的说明参考连接：https://blog.csdn.net/m0_74091159/article/details/139039490
    // 用于声明一个类的属性，并将其与类中的成员变量或成员函数关联起来。这使得属性可以通过Qt元对象系统进行访问和操作。
    Q_PROPERTY(int myValue READ myValue WRITE setmyValue NOTIFY myValueChanged) // 信号和槽的集成

public:
    explicit BrowserController(QObject* parent=nullptr);

    int myValue() const {return m_value;}

    void setmyValue(int value){
        if(value!=m_value) {
            m_value=value;
            emit myValueChanged();          // 表示一旦数据发生改变时，就会发生myValueChanged信号
        }
    }

signals:
    void myValueChanged();

private:
    int m_value;
};

#endif // BROWSERCONTROLLER_H
