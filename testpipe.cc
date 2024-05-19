#include<iostream>
#include<string>
#include<cerrno>//c++版本的errno.h
#include<cstring>//c++版本的string.h
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

//fork之后子进程是能拿到父进程的数据的 --- 这是通信吗？因为写时拷贝，父子双方任意一方修改对方都看不到
//char buffer[1024]想定义一个全局的buffer，一个从缓冲区中写，一个从缓冲区中拿是不行的

const int size = 1024;

//获取动态信息
std::string getOtherMessage()
{
    static int cnt = 0;
    std::string messageid = std::to_string(cnt);//stoi->string转int
    ++cnt;
    pid_t self_id = getpid();
    std::string stringPid = std::to_string(self_id);

    std::string message = "messageid: ";
    message += messageid;
    message += " my pid is:  ";
    message += stringPid;

    return message;
}
//子进程进行写入
void SubProcessWrite(int wfd)
{
    int pipesize =0;
    std::string message = "Father,I am your son process!";
    char c = 'A';

    while(true)
    {
        //子进程是一句一句写的
        //分割符写到标准错误文件中
        std::cerr<<"+++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        std::string info = message + getOtherMessage();//这条消息就是子进程要发给父进程的消息
        write(wfd,info.c_str(),info.size());//写入管道的时候，有没有写入\0，有没有必要？
                                            //以\0为结尾是C语言的标准，跟文件没关系，没必要写
        std::cerr<< info <<std::endl;
        
        //sleep(1);//子进程写慢一点，，方便看效果         

        // write(wfd,&c,1);
        // std::cout<<"pipesize: "<<++pipesize<<" write charator is: "<< c++ <<std::endl;  
        // if(c=='G')
        // {
        //     break;
        // }
        // sleep(1);                       
    }
    std::cout<<"child quit..."<<std::endl;
}
//父进程读取---与子进程保持一直，协同
void FatherProcessRead(int rfd)
{
    char inbuffer[size];//c99,gnu g99 标准支持
    while(true)
    {
        //父进程是一段一段读取的
        sleep(2);
        std::cout<<"--------------------------------------------------"<<std::endl;
        //sleep(500);
        ssize_t n = read(rfd,inbuffer,sizeof(inbuffer) - 1);//sizeof(inbuffer)->strlen(inbuffer);
        if(n > 0)
        {
            //有效字符串的最后一个元素的下一个位置
            inbuffer[n] = 0;//=='\0'
            std::cout<<"father get message: "<<inbuffer<<std::endl;
        }
        else if(n==0)
        {
            //如果read的返回值是0 表示写端直接关闭了，我们读到了文件的结尾
            std::cout<<"client quit,father get return val: "<< n <<" father quit too!"<<std::endl;
            break;
        }
        else if(n<0)
        {
            std::cerr<<"read error"<<std::endl;
            break;
        }
        sleep(1);
    break;
        //
    }
}
int main()
{
    //1.创建管道
    int pipefd[2];
    int n = pipe(pipefd);//输出型参数，rfd，wfd
    if(n != 0)//创建失败
    {
        std::cerr<<"errno: "<<errno<<": "<<"errstring : "<<strerror(errno)<<std::endl;
        return 1;
    }
    //pipefd[0]->0->read(嘴巴-读)    pipefd[1]->1->write(笔->写)
    std::cout<<"pipefd[0]: "<<pipefd[0]<<", pipefd[1]:"<<pipefd[1]<<std::endl;
    sleep(1);
    //2.创建子进程
    pid_t id = fork();
    if(id == 0)
    {
        std::cout<<"子进程关闭不需要的fd了,准备发送消息!"<<std::endl;
        sleep(1);
        //子进程---write
        //3.关闭不需要的fd
        close(pipefd[0]);
        //孙子进程
        //if(fork()>0) exit(0);
        SubProcessWrite(pipefd[1]);
        close(pipefd[1]);

        exit(0);
    }
    std::cout<<"父进程关闭不需要的fd了,准备收消息!"<<std::endl;
    sleep(1);
    //父进程---read
    close(pipefd[1]);
    FatherProcessRead(pipefd[0]);
    std::cout<<"5S,Father close rfd"<<std::endl;
    sleep(5);
    close(pipefd[0]);

    //防止僵尸进程
    // pid_t rid = waitpid(id,nullptr,0);//等子进程，阻塞
    // if(rid > 0)
    // {
    //     std::cout<<"wait child process done"<<std::endl;
    // }

    int status = 0;
    pid_t rid = waitpid(id,&status,0);
    if(rid > 0)
    {
        std::cout <<" wait child process done, exit sig: "<<(status&0x7f)<<std::endl;
        std::cout <<" wait child process done, exit code(ign): "<<((status>>8)&0x7f)<<std::endl;

    }

    return 0;
}