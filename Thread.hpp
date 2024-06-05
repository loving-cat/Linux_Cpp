#pragma once
#include<iostream>
#include<string>
#include<functional>
#include<pthread.h>


namespace ThreadMoudle
{
    //线程要执行的方法，随时调整
    //using func_t = std::function<void()>;//相当于typedef C++11新特性
    //typedef std::function<void()> func_t;
    //name 把名字带出来
    typedef void(*func_t)(const std::string &name);//函数指针类型

    class Thread
    {

    public:
        void Excute()
        {
            std::cout<<_name<<"is running"<<std::endl;
            _isrunning = true;
            _func(_name);
            _isrunning=false;
        }
    public:
        Thread(const std::string &name,func_t func);
        static void *ThreadRoutine(void *args)//新线程都会执行该方法！
        {
            Thread *self = static_cast<Thread*>(args);//获得当前对象
            self->Excute();
            return nullptr;
        }
        bool Start();
        std::string Status();
        void Stop();
        void Join();
        std::string Name();
        //~Thread();
    private:
        /* data */
        std::string _name;
        pthread_t _tid;
        bool _isrunning;
        func_t _func;//线程要执行的回调函数
    };

    Thread::Thread(const std::string &name,func_t func):_name(name),_func(func)
    {
        std::cout<<"create "<<name<<"done "<<std::endl;
    }

    bool Thread::Start()
    {
        // :: 强调的是使用标准库的调用
        int n = ::pthread_create(&_tid,nullptr,ThreadRoutine,this);
        if(n!=0) return false;
        return true;
    }
    std::string Thread::Status()
    {
        if(_isrunning) return "running";
        else return "sleep";
    }
    void Thread::Stop()
    {
        if(_isrunning)
        {
            ::pthread_cancel(_tid);
            _isrunning = false;
            std::cout<<_name<<"Stop"<<std::endl;
        }
    }
    void Thread::Join()
    {
        // if(!_isrunning)
        // {
        ::pthread_join(_tid,nullptr);
        std::cout << _name << " Joined" << std::endl;
        //}
    }
    std::string Thread::Name()
    {
        return _name;
    }

    // Thread::~Thread()
    // {

    // }
}
