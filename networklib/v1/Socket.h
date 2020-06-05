#pragma once

#include"Noncopyable.h"
namespace wd
{

class InetAddress;
class Socket:public Noncopyable
{
public:
    Socket(int sockfd);
    Socket();
    ~Socket();

    void shutdownWrite();  //关闭写
    int fd()const{return sockfd_;}

    void nonblock();  //设置非阻塞

    static InetAddress getLocalAddr(int sockfd);        //获取本地协议地址
    static InetAddress getPeerAddr(int sockfd);         //获取外地协议地址

private:
    int sockfd_;
};
}
