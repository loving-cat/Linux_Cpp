#include<iostream>
#include<string>
#include<pthread.h>
#include<unistd.h>
//只在Linux 有效，__thread只能修饰内置类型
__thread int gval = 100;
std::string ToHex(pthread_t tid)
{
    char id[128];
    snprintf(id,sizeof(id),"0x%x",tid);
    return id;
}

void* threadRun(void* args)
{
    std::string name = static_cast<const char*>(args);
    while(true)
    {
        std::string id = ToHex(pthread_self());
        std::cout<<name<<"is running,tid: "<<id<<",gval: "<<gval<<",&gval:"<<&gval<<std::endl;
        sleep(1);
        gval++;
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,nullptr,threadRun,(void*)"thread-1");
    while(true)
    {
        std::cout<<"new thread tid: "<<tid<<std::endl;
        sleep(1);
    }
    pthread_join(tid,nullptr);
    return 0;
}