#ifndef _SHM_HPP_
#define _SHM_HPP_

#include<iostream>
#include<string>
#include<cerrno>
#include<cstdio>
#include<cstring>
#include<sys/ipc.h>
#include<sys/shm.h>

const int gCreater =1;
const int gUser = 2;
const std::string gpathname="/root/code/4.shm";
const int gproj_id = 0x66;
const int gShmSize = 4096;

class Shm
{
private:
    key_t GetCommKey()
    {
        key_t k = ftok(_pathname.c_str(),_proj_id);
        if(k < 0)
        {
            perror("ftok");
        }
        return k;
    }
    int GetShmHelper(key_t key,int size,int flag)
    {
        int shmid = shmget(key,size,flag);
        if(shmid < 0)
        {
            perror("shmget");
        }
        return shmid;
    }
    std::string RoleToString(int who)
    {
        if(who == gCreater) return "Creater";
        else if(who == gUser) return "User";
        else return "None";
    }
    void *AttachShm()
    {
        if(_addrshm != nullptr)DetachShm(_addrshm);
        void *shmaddr = shmat(_shmid,nullptr,0);
        if(shmaddr == nullptr)
        {
            perror("shmat");
        }
        std::cout<<"who: "<<RoleToString(_who)<<"attach shm..."<<std::endl;
        return shmaddr;
    }
    void DetachShm(void *shmaddr)
    {
        if(shmaddr == nullptr) return;
        shmdt(shmaddr);
        std::cout<<"who: "<<RoleToString(_who)<<"Deatach shm..."<<std::endl;
    }
public:
    Shm(const std::string &pathname,int proj_id,int who)
    :_pathname(pathname),_proj_id(proj_id),_who(who),_addrshm(nullptr)
    {
        _key = GetCommKey();
        if(_who == gCreater)
        {
            GetShmUserCreate();
        }
        else if(_who=gUser)
        {
            GetShmForUser();
        }
        _addrshm=AttachShm();
        std::cout<<"shmid: "<<_shmid<<std::endl;
        std::cout<<"_key "<<ToHex(_key)<<std::endl;

    }
    ~Shm()
    {
        //创建者才能删除
        if(_who == gCreater)
        {
            int res = shmctl(_shmid,IPC_RMID,nullptr);
        }
        std::cout<<"shm remove done......."<<std::endl;
    }
    std::string ToHex(key_t key)
    {
        char buffer[128];
        snprintf(buffer,sizeof(buffer),"0x%x",key);
        return buffer;
    }
    bool GetShmUserCreate()
    {
        if(_who == gCreater)
        {
            _shmid = GetShmHelper(_key,gShmSize,IPC_CREAT|IPC_EXCL);
            if(_shmid >= 0) return true;
            std::cout<<"shm create done......."<<std::endl;

        }
        return false;
    }
    bool GetShmForUser()
    {
        if(_who == gUser)
        {
            _shmid = GetShmHelper(_key,gShmSize,IPC_CREAT|IPC_EXCL);
            if(_shmid >= 0) return true;
            std::cout<<"shm get done......."<<std::endl;

        }
        return false;
    }
    void Zero()
    {
        if(_addrshm)
        {
            memset(_addrshm,0,gShmSize);
        }
    }
    void *Addr()
    {
        return _addrshm;
    }
    void DebugShm()
    {
        struct shmid_ds ds;
        int n = shmctl(_shmid,IPC_STAT,&ds);
        if(n<0)return;
        std::cout<<"ds.shm_perm.__key: "<<ds.shm_perm.__key<<std::endl;
        std::cout<<"ds.shm_nattch:"<<ds.shm_nattch<<std::endl;
    }
private:
    key_t _key;
    int _shmid;
    const std::string _pathname;
    const int _proj_id;
    int _who;
    void *_addrshm;
};

// std::string ToHex(key_t k)
// {
//     char buffer[128];
//     snprintf(buffer,sizeof(buffer),"0x%x",k);
//     return buffer;
// }

// key_t GetCommKey(const std::string &pathname,int proj_id)
// {
//     key_t k = ftok(pathname.c_str(),proj_id);
//     if(k < 0)
//     {
//         perror("ftok");
//     }
//     return k;
// }

int ShmGet(key_t key,int size)
{
    int shmid = shmget(key,size,IPC_CREAT|IPC_EXCL);
    if(shmid < 0)
    {
        perror("shmget");
    }
    return shmid;
}


#endif // !_SHM_HPP_