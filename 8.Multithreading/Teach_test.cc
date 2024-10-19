#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void threadrun(std::string name, int num)
{
    while(num)
    {
        std::cout << name << " num : " << num<< std::endl;
        num--;
        sleep(1);
    }
}

int main()
{
    std::string name = "thread-1";
    std::thread mythread(threadrun, std::move(name), 10);
    while(true)
    {
        std::cout << "main thhread..." << std::endl;
        sleep(1);
    }
    mythread.join();
    return 0;
}


































// // 可以给线程传递多个参数，甚至方法了
// // class ThreadData
// // {
// // public:
// //     int Excute()
// //     {
// //         return x + y;
// //     }
// // public:
// //     std::string name;
// //     int x;
// //     int y;
// //     // other
// // };

// // class ThreadResult
// // {
// // public:
// //     std::string print()
// //     {
// //         return std::to_string(x) + "+" + std::to_string(y) + "=" + std::to_string(result);
// //     }
// // public:
// //     int x;
// //     int y;
// //     int result;
// // };

// // // 问题5: 全面看待线程函数返回: 
// // // a. 只考虑正确的返回，不考虑异常，因为异常了，整个进程就崩溃了，包括主线程 
// // // b. 我们可以传递任意类型，但你一定要能想得起来，也可以传递类对象的地址！！！
// // void *threadRun(void *args)
// // {
// //     //std::string name = (const char*)args;
// //     //int a = *(int*)args; // warning
// //     ThreadData *td = static_cast<ThreadData*>(args); // (ThreadData*)args
// //     ThreadResult *result = new ThreadResult();
// //     int cnt = 10;
// //     while(cnt)
// //     {
// //         sleep(3); 
// //         std::cout << td->name << " run ..." << ", cnt: " << cnt-- << std::endl;
// //         result->result = td->Excute();
// //         result->x = td->x;
// //         result->y = td->y;
// //         break;
// //         // int *p = nullptr;
// //         // *p = 100; // 故意野指针
// //     }
// //     delete td;
// //     return (void*)result;
// // }

// std::string PrintToHex(pthread_t &tid)
// {
//     char buffer[64];
//     snprintf(buffer, sizeof(buffer), "0x%lx", tid);
//     return buffer;
// }

// const int num = 10;

// void *threadrun(void *args)
// {
//     // pthread_detach(pthread_self());
//     std::string name = static_cast<const char*>(args);
//     while(true)
//     {
//         std::cout << name << " is running" << std::endl;
//         sleep(3);
//         // int *p = nullptr;
//         // *p = 100;
//         break;
//     }
//     // return args;
//     // exit(1); // 进程: 专门用来终止进程的，不能用来终止线程！
//     pthread_exit(args); // 专门终止一个线程的！
// }

// // main函数结束，main thread结束，表示进程结束！
// int main()
// {
//     // 问题7: 新线程如何终止？
//     // 1. 线程函数 return
//     // 2. pthread_exit
//     // 3. main thread call pthread_cancel, 新线程退出结果是-1

//     // 问题8: 可以不可以不join线程，让他执行完就退出呢？？可以！
//     // a. 一个线程被创建，默认是joinable的，必须要被join的.
//     // b. 如果一个线程被分离，线程的工作状态分离状态，不须要/不能被join的. 依旧属于进程内部，但是不需要被等待了

//     // std::vector<pthread_t> tids;
//     // // 问题6: 如何创建多线程呢？
//     // for(int i = 0; i < num; i++)
//     // {
//     //     // 1. 有线程的id
//     //     pthread_t tid;
//     //     // 2. 线程的名字
//     //     char *name = new char[128];
//     //     snprintf(name, 128, "thread-%d", i+1);
//     //     pthread_create(&tid, nullptr, threadrun, /*线程的名字*/name);

//     //     // 3. 保存所有线程的id信息 --- base
//     //     // tids.push_back(tid);
//     //     tids.emplace_back(tid);
//     // }

//     // for (auto tid : tids)
//     // {
//     //     pthread_detach(tid); // 主线程分离新线程，新线程必须存在
//     // }
    

//     // // //做我的事情
//     // // while(true)
//     // // {
//     // //     sleep(1);
//     // // }
//     // // // join todo
//     // for(auto tid : tids)
//     // {
//     //     // pthread_cancel(tid); // 取消
//     //     // std::cout << "cancel: " << PrintToHex(tid) << std::endl;
//     //     void *result = nullptr; // 线程被取消线程的退出结果是：-1 #define PTHREAD_CANCELED ((void *) -1)
//     //     int n = pthread_join(tid, &result);
//     //     // std::cout << PrintToHex(tid) << " quit... "<< std::endl;
//     //     std::cout << (long long int)result << " quit..., n : " << n << std::endl;
//     //     // delete (const char*)name;
//     // }
//     // sleep(100);

//     // pthread_t tid; // unsigned long int
//     // // 问题1: main 和 new 线程谁先运行？不确定
//     // // 问题4：全面看待线程函数传参: 我们可以传递任意类型，但你一定要能想得起来，也可以传递类对象的地址！！！
//     // ThreadData *td = new ThreadData();
//     // td->name = "thread-1";
//     // td->x = 10;
//     // td->y = 20;

//     // int n = pthread_create(&tid, nullptr, threadRun, td);

//     // // td.name = "thread-2";
//     // // td.num = 2;
//     // // int n = pthread_create(&tid, nullptr, threadRun, (void*)&td);
//     // if(n != 0) // 后面我们暂时就关心了
//     // {
//     //     std::cerr << "create thread error" << std::endl;
//     //     return 1;
//     // }

//     // // 问题3: tid 是什么样子的？是什么呢？虚拟地址！ 为什么? TODO
//     // std::string tid_str = PrintToHex(tid); // 我们按照16进行打印出来
//     // std::cout << "tid : " << tid_str << std::endl;


//     // std::cout << "main thread join begin..." << std::endl;
//     // // 问题2：我们期望谁最后退出？ main thread,你如何保证呢？
//     // ThreadResult *result = nullptr; // 开辟了空间的！！！
//     // n = pthread_join(tid, (void**)&result); // join来保证。 不join呢？会造成类似僵尸进程的问题
//     // if(n == 0)
//     // {
//     //     std::cout << "main thread wait success, new thread exit code: " << result->print() << std::endl;
//     // }

//     // sleep(100);
//     return 0;
// }