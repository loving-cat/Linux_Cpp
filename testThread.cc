#include<iostream>
#include<string>
#include<pthread.h>
#include<unistd.h>

class ThreadData
{
public:
    std::string name;
    int num;
    //other
};

//问题5：全面看待线程函数返回
void *threadRun(void *args)
{
    //int a = *(int*)args;
    //               安全类别的强转
    ThreadData *td = static_cast<ThreadData*>(args);//(ThreadData*)args
    int cnt = 5;
    while(cnt)
    {
        std::cout<< td->name <<"new thread run ....,num is "<<td->num<<",cnt: "<<cnt--<<std::endl;
        sleep(1);
    }
    delete td;
    return (void*)0;
    // std::string name = (const char*)args;
    // int cnt = 5;
    // while(cnt)
    // {
    //     std::cout<<"name"<<name<<"new thread run ....,cnt :"<<cnt--<<std::endl;
    //     sleep(1);
    // }
}
std::string PrintToHex(pthread_t &tid)
{
    char buffer[64];
    snprintf(buffer,sizeof(buffer),"0x%x",tid);
    return buffer;
}
int main()
{
    pthread_t tid;//unsigned long int
    //问题1：main和new线程谁先运行？不确定
    //问题4：全面看待线程函数传参:我们可以传递任意类型，但是要想得起来，可以传递类对象的地址
    //推荐在堆上开空间
    ThreadData *td = new ThreadData();
    td->name = "thread-1";
    td->num = 1;
    //int a = 100;
    int n = pthread_create(&tid,nullptr,threadRun,td);

    //不推荐
    // td.name = "thread-2";
    // td.num = 2;
    // int n = pthread_create(&tid,nullptr,threadRun,(void*)&td);

    //int n = pthread_create(&tid,nullptr,threadRun,(void*)"thread 1");
    if(n != 0)// 后面不写了，不然创建一个判断一个消耗太大
    {
        std::cerr<<"create thread error"<<std::endl;
        return 1;
    }
    //问题3：tid是什么样子的？是什么呢？虚拟地址！ 为什么？ TODO
    std::string tid_str = PrintToHex(tid);//按照16进制进行打印
    std::cout<<"tid:"<<tid_str<<std::endl;
    std::cout<<"main thread join begin"<<std::endl;
    //问题2：我们期望谁最后退出？ main thread,你如何保证呢？
    void *code = nullptr;//开辟了空间的
    n = pthread_join(tid,&code);//join来保证。不join呢？ 不影响，主线程提前退了其他线程也退了
    if(n==0)
    {
        std::cout<<"main thread wait success,new thread exit code: "<<(uint64_t)code<<std::endl;
    }
    return 0;
}