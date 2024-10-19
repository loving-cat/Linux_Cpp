#include<iostream>
#include<unistd.h>

void fun()
{

}
//新线程
void *threadStart(void *args)
{
    int x = rand()%5;
    while(true)
    {
        std::cout<<"new thread running"<<",pid: "<<getpid()<<std::endl;
        sleep(1);
        fun();
        // if(x==0)
        // {
        //     int *p = nullptr;
        //     *p =  100;//野指针    
        // }
    }

}

int main()
{
    pthread_t tid1;
    pthread_create(&tid1,nullptr,threadStart,(void *)"thread-new");
    pthread_t tid2;
    pthread_create(&tid2,nullptr,threadStart,(void *)"thread-new");
    pthread_t tid3;
    pthread_create(&tid3,nullptr,threadStart,(void *)"thread-new");
    //主线程
    while(true)
    {
        sleep(1);
        std::cout<<"main thread running"<<",pid: "<<getpid()<<std::endl;
    }
    return 0;
}