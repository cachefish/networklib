#ifndef __SOCKET_H__
#define __SOCKET_H__

#include"SocketUtil.h"
#include"InetAddress.h"
#include"Socket.h"
#include"Noncopyable.h"

namespace wd
{

Socket::Socket( int sockfd):sockfd_(sockfd)
{

}

Socket::Socket():sockfd_(createSocketfd())
{
 
}

Socket::~Socket()
{
    ::close(sockfd_);
}

void Socket::shutdownWrite()
{
    if(::shutdown(sockfd_,SHUT_WR)==-1)
    {
        perror("shutdown write error!");
    }
}

InetAddress Socket::getLocalAddr(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if(getsockname(sockfd,(struct sockaddr*)&addr,&len)==-1)
    {
        perror("getpeername error");
    }
    return InetAddress(addr);
}



}

#endif