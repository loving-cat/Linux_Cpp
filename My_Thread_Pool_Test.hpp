#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<pthread.h>
#include<unistd.h>
#include<functional>
#include<exception>


//任务封装结构体，包含任务函数和参数
struct Task
{
    std::function<void()> func;
    Task(){}
    Task(std::function<void()> f) : func(f)
    {}
};
//线程池类定义
class ThreadPool
{
private:
void Work()
{
    while(true)
    {
        Task task;
        pthread_mutex_lock(&QueueMutex);//锁定互斥区
        while(TaskQueue.empty()&&!stop)
        {
            //如果任务队列为空且没有停止，则等待
            pthread_cond_wait(&cond,&QueueMutex);
        }
        if(stop && TaskQueue.empty())
        {
            //如果停止且任务队列为空，则退出循环
            pthread_mutex_unlock(&QueueMutex);
            break;
        }
        task = TaskQueue.front();//获取任务
        TaskQueue.pop();//从队列中移除任务
        pthread_mutex_unlock(&QueueMutex);//解锁互斥区
        try
        {
            task.func();//执行任务
        }
        catch(const std::exception& e)
        {
            std::cerr<<"Exception in thread:"<<e.what()<<std::endl;
        }
    }
}
public:
    //初始化线程池
    ThreadPool(size_t threadsNum):stop(false)
    {
        pthread_mutex_init(&QueueMutex,nullptr);
        pthread_cond_init(&cond,nullptr);
        for(size_t i = 0; i < threadsNum; ++i)
        {
            if(pthread_create(&threads[i],nullptr,&ThreadPool::threadWrapper,this)!= 0)
            {
                std::cout<<"Error creating thread"<<std::endl;
            }
        }
    }
    ~ThreadPool()
    {
        //创建局部作用域，确保内部代码尽快执行
        {
            pthread_mutex_lock(&QueueMutex);
            stop = true;
            pthread_cond_broadcast(&cond);//唤醒所有等待的线程
            pthread_mutex_unlock(&QueueMutex);
        }
        for(pthread_t &thread : threads)
        {
            pthread_join(thread,nullptr);//等待线程结束
        }
        pthread_mutex_destroy(&QueueMutex);
        pthread_cond_destroy(&cond);
    }
    //添加任务到线程池
    void addTask(std::function<void()> taskFunc)
    {
        Task task(taskFunc);
        pthread_mutex_lock(&QueueMutex);
        TaskQueue.push(std::move(task));
        pthread_cond_signal(&cond);//通知一个线程有任务可以执行
        pthread_mutex_unlock(&QueueMutex);
    }
    
    static void *threadWrapper(void *args)
    {
        ThreadPool* pool = static_cast<ThreadPool*>(args);
        pool->Work();
        return nullptr;
    }
private:
    //线程列表
    std::vector<pthread_t> threads;
    //任务队列
    std::queue<Task> TaskQueue;
    //互斥锁，保护任务队列
    pthread_mutex_t QueueMutex;
    //条件变量，用于线程同步
    pthread_cond_t cond;
    //停止标志
    bool stop;
};