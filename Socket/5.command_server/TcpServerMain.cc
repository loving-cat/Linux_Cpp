#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <set>
#include "Log.hpp"
#include "InetAddr.hpp"
using namespace log_ns;
class Command
{
public:
    Command()
    {
        // 白名单
        _safe_command.insert("ls");
        _safe_command.insert("touch"); // touch filename
        _safe_command.insert("pwd");
        _safe_command.insert("whoami");
        _safe_command.insert("which"); // which pwd
    }
    ~Command() {}
    // ls;rm -rf /
    bool SafeCheck(const std::string &cmdstr)
    {
        for(auto &cmd : _safe_command)
        {
            if(strncmp(cmd.c_str(), cmdstr.c_str(), cmd.size()) == 0)
            {
                return true;
            }
        }

        return false;
    }
    std::string Excute(const std::string &cmdstr)
    {
        if(!SafeCheck(cmdstr))
        {
            return "unsafe";
        }
        std::string result;
        FILE *fp = popen(cmdstr.c_str(), "r");
        if(fp)
        {
            char line[1024];
            while(fgets(line, sizeof(line), fp))
            {
                result += line;
            }
            return result.empty() ? "success" : result;
        }
        return "execute error";
    }
    void HandlerCommand(int sockfd, InetAddr addr)
    {
        // 我们把他当做一个长服务
        while (true)
        {
            char commandbuf[1024]; // 当做字符串, ls -a -l -> os --> ls -
            ssize_t n = ::recv(sockfd, commandbuf, sizeof(commandbuf) - 1, 0); // TODO
            if (n > 0)
            {
                commandbuf[n] = 0;
                LOG(INFO, "get command from client %s, command: %s\n", addr.AddrStr().c_str(), commandbuf);
                std::string result = Excute(commandbuf);
                ::send(sockfd, result.c_str(), result.size(), 0);
            }
            else if (n == 0)
            {
                LOG(INFO, "client %s quit\n", addr.AddrStr().c_str());
                break;
            }
            else
            {
                LOG(ERROR, "read error: %s\n", addr.AddrStr().c_str());
                break;
            }
        }
    }
private:
    std::set<std::string> _safe_command;
};