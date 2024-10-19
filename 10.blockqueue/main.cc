#include"BlockQueue.hpp"

void *Consumer(void *args)
{
    BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(args);
    while(true)
    {
        //消费
        int data = bq->pop();
        std::cout<<"消费了一个数据："<< data << std::endl;

    }
}
void *Productor(void *args)
{
    BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(args);
    int data = 0;
    while(true)
    {
        //生产
        data++;
        bq->push(data);
        std::cout<<"生产了一个数据："<< data << std::endl;
    }
}


int main()
{
    //BlockQueue 内部可不可以传递其他数据，比如对象？比如任务？？？
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_t c,p;
    pthread_create(&c,nullptr,Consumer,nullptr);
    pthread_create(&p,nullptr,Productor,nullptr);

    //线程等待
    pthread_join(c,nullptr);
    pthread_join(p,nullptr);
    delete bq;
    return 0;
}