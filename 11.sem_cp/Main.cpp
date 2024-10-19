#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include"RingQueue.hpp"

using namespace std;

struct ThreadData
{
    RingQueue<int> *rq;
    std::string threadname;
};

void *Productor(void *args)
{
    //sleep(3);
    ThreadData *td = static_cast<ThreadData*>(args);
    RingQueue<int> *rq = td->rq;
    std::string name = td->threadname;
    while(true)
    {

        //1.获取数据
        int data = rand()%10+1;

        //2.生产数据
        rq->Push(data);
        std::cout<<"Productor..."<<std::endl;
        sleep(1);
    }
    return nullptr;
}

void *Consumer(void *args)
{
    ThreadData *td = static_cast<ThreadData*>(args);
    RingQueue<int> *rq = td->rq;
    std::string name = td->threadname;
    //RingQueue<int> *rq = static_cast<RingQueue<int> *>(args);
    while(true)
    {
        //1.消费数据
        int data = 0;
        rq->Pop(&data);

        std::cout<<"Consumer..."<<std::endl;
        sleep(1);
    }
    return nullptr;
}


int main()
{
    srand(time(nullptr)^getpid());
    RingQueue<int> *rq = new RingQueue<int>();

    pthread_t c[5], p[3];

    for(int i = 0 ;i < 3;i++)
    {
        ThreadData *td = new ThreadData();
        td->rq = rq;
        td->threadname = "Productor-" + std::to_string(i);
        pthread_create(p+i,nullptr,Productor,rq);
    }
    for(int i = 0; i < 5; i ++)
    {
        ThreadData *td = new ThreadData();
        td->rq = rq;
        td->threadname = "Consunmer-" + std::to_string(i);
        pthread_create(c+i,nullptr,Consumer,rq);
    }

    for(int i = 0 ;i < 3;i++)
    {
        pthread_join(c[i],nullptr);
    }
    for(int i = 0; i < 5; i ++)
    {
        pthread_join(p[i],nullptr);
    }
    return 0;
}