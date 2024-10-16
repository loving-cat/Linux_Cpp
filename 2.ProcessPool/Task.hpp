#pragma once

#include<iostream>
#include<ctime>
#include<random>
#include<sys/types.h>
#include<unistd.h>
#define TaskNum 3

//返回值为void 参数为()的函数指针
typedef void (*task_t)();//tast_t 函数指针类型
task_t tasks[TaskNum];


void Print()
{
    std::cout<<"I AM PRINT TASK"<<std::endl;
}

void DownLoad()
{
    std::cout<<"I AM A DOWNLOAD TASK"<<std::endl;
}

void Flush()
{
    std::cout<<"I AM A FLUSH TASK"<<std::endl;
}
task_t tasks[TaskNum];
//加载
void LoadTask()
{
    srand(time(nullptr)^getpid()^17777);
    tasks[0]=Print;
    tasks[1]=DownLoad;
    tasks[2]=Flush;
}
//执行
void ExcuteTask(int number)
{
    if(number < 0 || number > 2) 
        return;
    tasks[number]();
}
int SelectTask()
{
    return rand() % TaskNum;
}