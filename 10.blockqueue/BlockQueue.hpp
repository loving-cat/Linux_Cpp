#pragma once
#include<iostream>
#include<queue>

template<class T>
class BlockQueue
{
    static const int defalutnum = 5;
public:
    BlockQueue(int maxcap = defalutnum):maxcap_(maxcap),
    {
        pthread_mutex_init(&mutex_,nullptr);
        pthread_cond_init(&c_cond_,nullptr);
        pthread_cond_init(&p_cond_,nullptr);
        low_water_ = maxcap_/3;
        high_water_ = (maxcap_*2)/3;
    }
    //谁来唤醒呢？
    T pop()
    {
        pthread_mutex_lock(&mutex_);
        if(q_.size()==0)
        {
            pthread_cond_wait(&c_cond_,&mutex_);
        }
        T out = q_.front();//你想消费，就直接能消费吗？不一定，你得先确保消费条件满足
        q_.pop();
        if(q_.size()>high_water_) pthread_cond_signal(&p_cond_);
        pthread_mutex_ubnlock(&mutex_);

        return out;
    }
    void push(const T &in)
    {
        pthread_mutex_lock(&mutex_);
        if(q_.size() == maxcap_)
        {
            pthread_cond_wait(&p_cond_,&mutex_);//1.调用的时候，自动释放锁 2.
        }
        //1.队列没满 2.被唤醒
        q_.push(in);//你想生产，就直接能生产吗？ 不一定，你得先确保生产条件满足
        pthread_cond_signal(&c_cond_);
        pthread_mutex_ubnlock(&mutex_);
    }
    ~BlockQueue()
    {
        pthread_murtex_destroy(&mutex_);
        pthread_cond_destroy(&c_cond_);
        pthread_cond_destroy(&p_cond_);

    }
private:
    std::queue<T> q_;
    int maxcap_;//极值
    pthread_mutex_t mutex_;
    pthread_cond_t c_cond_;
    pthread_cond_t p_cond_;

    int low_water_;
    int high_water_;
};