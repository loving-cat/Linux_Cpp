#include<iostream>
#include<unistd.h>
#include<cstdio>
#include<sys/types.h>
#include<sys/wait.h>
void PrintPending(sigset_t &pending)
{
    std::cout<<"curr process["<<getpid()<<"]pending";
    for(int signo = 31;signo >= 1; signo--)
    {
        if(sigismember(&pending,signo))
        {
            std::cout<<1;
        }
        else
        {
            std::cout<<0;
        }
    }
    std::cout<<"\n";
}

void handler(int signo)
{
    std::cout<<signo<<"号信号被递达"<<std::endl;
    std::cout<<"--------------------------------"<<std::endl;
    sigset_t pending;
    sigpending(&pending);
    PrintPending(pending);
    std::cout<<"--------------------------------"<<std::endl;

}

int main()
{
    //0.捕捉2号信号
    signal(2,handler);//自定义捕捉
    signal(2,SIG_IGN);//忽略一个信号
    signal(2,SIG_DFL);//信号的默认处理动作
    
    //1.屏蔽2号信号
    sigset_t block_set,old_set;
    sigemptyset(&block_set);
    sigemptyset(&old_set);
    sigaddset(&block_set,SIGINT);//到目前为止，我们有没有修改当前进程在系统内部的block表呢？ 没有
    //1.1设置进入进程bolck表中
    sigprocmask(SIG_BLOCK,&block_set,&old_set);//真正的修改当前进行的内核block表，完成了对2号信号的屏蔽!
    int cnt = 5;
    while(true)
    {
        //2.获取当前进程的pending信号集
        sigset_t pending;
        sigpending(&pending);        
        //3打印pending信号集
        PrintPending(pending);
        cnt--;
        //4.解除对2号信号的屏蔽
        if(cnt == 0)
        {
            std::cout<< "解除对2号信号的屏蔽！！！ "<<std::endl;
            sigprocmask(SIG_SETMASK,&old_set,&block_set);
        }
        sleep(1);
    }
    return 0;
}




// struct bits
// {
//     uint32_t bits[400];//400*32
// };
//第40个比特位如何访问
//40/(sizeof(uin32_t)*8) = 1 -> bits[1]
//40%(sizeof(uin32_t)*8) = 8 -> bits[1]:8

// int main()
// {
//     sigset_t bits;//sigset_t Linux给用户提供一个用户级的数据类型，禁止用户直接修改范围
//     return 0;
// }





// int Sum(int start,int end)
// {
//     int sum =0;
//     for(int i = start;i <= end;++i)
//     {
//         sum /= 0; //发生核心转储 报错 8号信号
//         sum+=i;
//     }
//     return sum;
// }

// int main()
// {
//     int total = Sum(0,100);
//     std::cout << "total: "<<total<<std::endl;
//     return 0;
// }