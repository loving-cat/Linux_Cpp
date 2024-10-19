#include<iostream>
#include<pthread.h>
#include<unistd.h>

int cnt = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *Count(void * args)
{
    pthread_detach(pthread_self());
    uint64_t number = *(uint64_t*) args;
    while(true)
    {
        pthread_mutex_lock(&mutex);
        //我们怎么知道我们要让一个线程去休眠呢？
        pthread_cond_wait(&cond,&mutex);//为什么在这里等？ 1.pthread_cond_wait让线程等待的时候，会自动释放锁
        //不管临界资源的状态情况
        std::cout<<"pthread: "<<number<<", cnt: "<< cnt++ <<std::endl;
        sleep(3);
        pthread_mutex_unlock(&mutex);

    }
}

int main()
{
    for(uint64_t i = 0;i < 5; i++)
    {
        pthread_t tid;
        pthread_create(&tid,nullptr,Count,(void*)&i);
    }

    while(true)
    {
        sleep(1);
        pthread_cond_signal(&cond); //唤醒在cond的等待队列中等待的一个线程，默认都是第一个
        pthread_cond_broadcast(&cond);
        std::cout<<"signal one thread.. "<<std::endl;
    }
    return 0;

}