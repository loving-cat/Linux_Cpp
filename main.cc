#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
//4 -----------------------------------------------------------------------------

int main()
{
    signal(SIGCHLD,SIG_IGN);//收到设置对SIGCHLD进行忽略即可
    pid_t id = fork();
    if(id==0)
    {
        int cnt = 5;
        while(cnt)
        {
            std::cout<<"child running"<<std::endl;
            cnt--;
            sleep(1);
        }
        exit(1);
    }
    while(true)
    {
        std::cout<<"father running"<<std::endl;
        sleep(1);
    }
    return 0;
}










//4 -----------------------------------------------------------------------------


// //3-------------------------------------------------------------------------------------
// //挑问题！
// //问题1：如果一共有10个子进程，且同时退出呢？
// //问题2：如果一共有10个子进程，5个退出，5个永不退出
// void notice(int signo)
// {
//     std::cout<< "get a signal: "<<signo<<" pid: "<<getpid()<<std::endl;
//     while(true)
//     {
//         pid_t rid = waitpid(-1,nullptr,WNOHANG);//-1是等待任意一个子进程退出
//         if(rid > 0)
//             std::cout<<"wait child sucess,rid: "<< rid <<std::endl;
//         else if(rid < 0)
//         {
//             std::cout<<"wait child success done"<< rid <<std::endl;
//             break;
//         }
//         else
//         {
//             std::cout << " "<<std::endl;
//             break;
//         }
//     }

// }

// void DoOtherThing()
// {
//     std::cout<<"DoOtherThing~"<<std::endl;
// }

// int main()
// {
//     signal(SIGCHLD,notice);
//     for(int i = 0; i < 10;i++)
//     {
//         pid_t id = fork();
//         if(id == 0)
//         {
//             std::cout<<"I am child process,pid: "<<getpid()<<std::endl;
//             sleep(3);
//             exit(1);
//         }
//     }
    
//     //father
//     while(true)
//     {
//         DoOtherThing();
//         sleep(1);
//     }
//     sleep(100);
//     return 0;
// }
// //3-------------------------------------------------------------------------------------

// //2-------------------------------------------------------------------------
// volatile int gflag =0;//不优化
// void changedata(int signo)
// {
//     std::cout<<"get a signo: "<<signo<<", change gflag 0->1 "<<std::endl;
//     gflag = 1;
// }

// int main()
// {
//     signal(2,changedata);
//     while(!gflag);
//     std::cout<<"process quit normal"<<std::endl;
// }
// //2-------------------------------------------------------------------------

// //1----------------------------------------------------------------------
// void Print(sigset_t &pending)
// {
//     for(int sig = 31; sig > 0; sig--)
//     {
//         if(sigismember(&pending,sig))
//             std::cout<<1;
//         else
//             std::cout<<0;
//     }
//     std::cout<<std::endl;
// }

// //当前如果正在对n号信号进程处理，默认n号信号会被自动屏蔽
// //对n号信号处理完成的时候，会自动解除对n号信号的屏蔽
// //为什么？
// void handler(int signum)
// {
//     //收到哪个信号就打印哪个信号
//     std:: cout<<"get a sig: "<<signum<<std::endl;
//     while(true)
//     {
//         sigset_t pending;
//         sigpending(&pending);
//         Print(pending);
//         sleep(1);
//     }
//     //sleep(100);
//     exit(1);
// }
// //信号捕捉
// int main()
// {
//     //这段代码的目的是设置当SIGINT信号发生时，
//     //将调用handler函数来处理这个信号，并且在处理信号期间不会阻塞任何其他信号。
//     //同时，它还保留了信号之前的处理动作，以便在需要时可以恢复。


//     //这行代码声明了两个sigaction结构体变量，
//     //act和oact。sigaction结构体用于定义信号处理的动作和相关属性。
//     struct sigaction act,oact;

//     //这行代码设置了act结构体的sa_handler成员，
//     //将其指向一个信号处理函数handler。当指定的信号被触发时，
//     //将调用这个函数。handler是一个函数指针，需要在代码中定义。  
//     act.sa_handler = handler;

//     //将act结构体的sa_mask成员清空。sa_mask是一个信号集，
//     //定义了在信号处理函数执行期间应该被阻塞的信号集合。
//     //清空这个集合意味着在处理信号时不会阻塞任何信号。
//     sigemptyset(&act.sa_mask);//如果你还想处理2号(OS对2号自动屏蔽)，同时对其他信号也进行屏蔽
//     sigaddset(&act.sa_mask,3);//指定添加3号信号
    

//     //这行代码设置了act结构体的sa_flags成员为0。sa_flags用于指定一些信号处理的行为标志。
//     //将这个值设置为0表示使用默认的行为。
//     act.sa_flags = 0;

//     //屏蔽所有信号//OS不允许全部屏蔽
//     for(int i = 0; i <= 31; i++)
//         sigaction(i,&act,&oact);

//     //这行代码调用了sigaction函数，用于设置或获取信号处理的相关信息。
//     //函数的第一个参数是信号的编号，这里是2，表示SIGINT信号，
//     //也就是通常由用户按下Ctrl+C产生的中断信号。第二个参数是指向新信号处理动作的指针&act，
//     //第三个参数是指向一个sigaction结构体的指针
//     sigaction(2,&act,&oact);
// //sigaction函数的作用是将act中定义的信号处理动作应用到指定的信号上，
// //同时将信号之前的处理动作存储到oact中。这样，如果需要，可以恢复到之前的信号处理行为。
//     while(true)
//     {
//         std::cout<<"I am a process, pid: "<<getpid()<<std::endl;
//         sleep(1);
//     }
//     return 0;
// }
// //1----------------------------------------------------------------------
