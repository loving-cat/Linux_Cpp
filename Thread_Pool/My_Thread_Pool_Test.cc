#include"My_Thread_Pool_Test.hpp"


void sampleTask()
{
    std::cout<<"任务在线程里啦！"<<pthread_self()<<std::endl;
    sleep(1);
    std::cout<<"完成任务啦"<<pthread_self()<<std::endl;
}
int main()
{
    const size_t numThreads = 4;
    ThreadPool pool(numThreads);

    for(int i = 0; i < 10; ++i)
    {
        pool.addTask(std::bind(sampleTask));
    }
    sleep(5);
    return 0;
}