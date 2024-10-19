#pragma once

#include<iostream>
#include<pthread.h>
#include<vector>
#include<queue>
#include<unistd.h>
#include<string>

struct ThreadInFo
{
    pthread_t tid;
    std::string name;
};
static const int defalutnum = 5;

template<class T>
class ThreadPool
{
public:
    void Lock()
    {
        pthread_mutex_lock(&mutex_);
    }
    void UnLock()
    {
        pthread_mutex_unlock(&mutex_);
    }
    void Wakeup()
    {
        pthread_cond_signal(&cond_);
    }
    void ThreadSleep()
    {
        pthread_cond_wait(&cond_,&mutex_);
    }
    bool IsQueueEmpty()
    {
        return tasks_.empty();
    }
    std::string GetThreadName(pthread_t tid)
    {
        for(const auto &ti : threads_)
        {
            if(ti.tid == tid) return ti.name;
        }
        return "None";
    }
public:
    ThreadPool(int num = defalutnum):threads_(num)
    {
        pthread_mutex_init(&mutex_,nullptr);
        pthread_cond_init(&cond_,nullptr);

    }
    //静态成员方法，不能直接访问类内的成员函数和成员变量
    static void *HandlerTask(void* args)//类内类自带函数指针this ThreadPool* this，static后就没有this指针了
    {
        ThreadPool<T> *tp = static_cast<ThreadPool<T> *> (args);
        std::string name = tp->GetThreadName(pthread_self());
        while(true)
        {
            tp->Lock();
            while(tp->IsQueueEmpty())
            {
                tp->ThreadSleep();
            }
            T t = tp->pop();
            tp->UnLock();
            t();
            std::cout<< name << "run," <<"result: "<<t.GetResult()<<std::endl;
        }
    }
    void Start()
    {
        int num = threads_.size();
        for(int i = 0; i < num; i++)
        {
            threads_[i].name = "thread-"+std::to_string(i + 1);
            pthread_create(&(threads_[i].tid),nullptr,HandlerTask,this);
        }
    }
    void Push(const T& t)
    {
        Lock();
        tasks_.push(t);
        Wakeup();
        UnLock();
    }
    T pop()
    {
        T t = tasks_.front();
        tasks_.pop();
        return t;
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);

    }
private:
    std::vector<ThreadInFo> threads_;//所有线程维护的数组
    std::queue<T> tasks_;//一批任务

    pthread_mutex_t mutex_;//保护临界资源
    pthread_cond_t cond_;//条件变量，对线程做唤醒
};