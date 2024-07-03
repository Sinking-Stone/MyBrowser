#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
public:
    static T& GetInstance(){
        static T m_instance;
        return m_instance;
    }
protected:
    Singleton()=default;
    ~Singleton()=default;

    Singleton(const Singleton&)=delete;
    Singleton& operator=(const Singleton&)=delete;
};

#endif // SINGLETON_H
