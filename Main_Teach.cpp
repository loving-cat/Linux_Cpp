#include"Teach_ThreadPool.hpp"
#include"Task.hpp"
#include<ctime>
int main()
{
    ThreadPool<Task> *tp = new ThreadPool<Task>(5);
    tp->Start();
    srand(time(nullptr) ^ getpid());
    while(true)
    {
        //1.构建任务
        int x = rand() % 10 + 1;
        usleep(10);
        int y = rand() % 5;
        char op = opers[rand()%opers.size()];
        Task t(x,y,op);
        tp->Push(t);
        //2.交给线程池处理
        std::cout<<"main thread make task"<<t.GetTask() <<std::endl;
        sleep(1);
    }
    return 0;
}