#include<iostream>
#include<unistd.h>
#include<vector>
#include"Thread.hpp"
using namespace ThreadMoudle;


int tickets = 10000;

void route(const std::string &name)
{
    while(true)
    {
        if(tickets>0)
        {
            //抢票过程
            usleep(1000);//微秒级休眠函数 1ms = 1000微秒-->抢票花费的时间
            printf("who:%s,get a ticket: %d\n",name.c_str(),tickets);
            tickets--;
        }
        else
        {
            break;
        }
    }
}

int main()
{
    Thread t1("thread-1",route);
    Thread t2("thread-2",route);
    Thread t3("thread-3",route);
    Thread t4("thread-4",route);

    t1.Start();
    t2.Start();
    t3.Start();
    t4.Start();

    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();


    return 0;
}















// void Print(const std::string name)
// {
//     int cnt = 1;
//     while(true)
//     {
//         std::cout<<name<<"is running,cnt: "<<cnt++<<std::endl;
//     }
// }

// //线程共享的全局变量
// const int gnum = 10;
// int mian()
// {
//     //我在管理原生线程，
//     //构建线程
//     std::vector<Thread> threads;
//     for(int i = 0;i < gnum;i++)
//     {
//         std::string name = "thread-1";
//         threads.emplace_back(name,Print);
//     }
//     //统一启动
//     for(auto &thread:threads)
//     {
//         thread.Start();
//     }
//     //统一结束
//     for(auto &thread:threads)
//     {
//         thread.Stop();
//     }

//     //等待线程结束
//     for(auto &thread:threads)
//     {
//         thread.Join();
//     }
//     // Thread t("thread-1",Print);
//     // t.Start();

//     // std::cout<<t.Name()<<",status: "<<t.Status()<<std::endl;
//     // sleep(10);
//     // std::cout<<t.Name()<<",status: "<<t.Status()<<std::endl;

//     // t.Stop();
//     // sleep(1);
//     // std::cout<<t.Name()<<",status: "<<t.Status()<<std::endl;

//     // t.Join();
//     // std::cout<<"join done"<<std::endl;

//     return 0;
// }