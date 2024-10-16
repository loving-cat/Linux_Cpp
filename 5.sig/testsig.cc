#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<cstdlib>
#include<sys/types.h>


void handler(int sig)
{
    std::cout<<"yc"<<std::endl;
}
int main()
{
    //
    //程序为什么会崩溃？？？[非法访问,操作（？），导致OS给进程发送信号啦！] 崩溃了为什么会退出？？可以不退出码？
    signal(SIGFPE,handler);
    while(true)
    {
        std::cout<<"hello bit,pid: "<<getpid()<<std::endl;
        int a = 10;
        a /= 0;//SIGFPE
        // int *p =nullptr;
        // *p = 100;//SIGSEGV
    }
}

// int cnt =1;

// void handler(int sig)
// {
//     std::cout<<"get a sig: "<< sig <<std::endl;
//      exit(1);
// }
// int main()
// {
//     int cnt = 0;
//     //signal(SIGABRT,handler);

//     for(int i = 1; i <= 31;i++)
//         signal(i,handler);

//     alarm(1);//设定1S后的闹钟 -- 1S ---SIGALRM
//     //sleep(4);
//     // int n = alarm(2);//alarm(0),取消闹钟，上一个闹钟的剩余时间
//     // std::cout<<"n:"<<n<<std::endl;
//     while(true)
//     {
//         sleep(1);
//         //raise(3);
//         //abort();
//     }
// }
//./mykill 2 1234
// int main(int argc,char *argv[])
// {
//     if(argc != 3)
//     {
//         std::cerr <<"Usage: "<<argv[0]<<"signum pid"<<std::endl;
//     }
//     pid_t pid = std::stoi(argv[2]);
//     int signum = std::stoi(argv[1]);
//     kill(pid,signum);
//     return 0;
// }


// int main()
// {
//     //对信号的自定义捕捉，我们只要捕捉一次，后续一直有效
//     //1.一直不产生信号呢?hander方法不调用
//     //2.可不可以对更多的信号进行捕捉呢？可以，给不同信号传递同样的hander方法，此时参数收到对应信号时，会自动填充对应信号
//     //3.2 SIGINT 默认是什么动作呢？终止进程
//     //4.2 SIGINT是什么呢？ ctrl+c --- 给目标进程发送2号信号，SIGINT默认是终止进程。
//     signal(2,hander);
//     //signal(3,hander);
//     //signal(4,hander);
//     //signal(5,hander);

//     while(true)
//     {
//         std::cout<<"hello bit"<<std::endl;
//         sleep(1);
//     }
// }