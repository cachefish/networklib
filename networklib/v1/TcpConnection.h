#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include"Noncopyable.h"
#include"InetAddress.h"
#include"Socket.h"
#include"SocketIO.h"

#include<string>
#include<memory>
#include<functional>

namespace wd
{

class TcpConnection:public Noncopyable
{
public:
    TcpConnection(int sockfd);
    ~TcpConnection();

    std::string receive();
    void send(const std::string &msg);
    void shutdown();

    std::string toString();

private:
    Socket sockfd_;
    SocketIO sockIO_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    bool isShutdownWrite_;
};
}

#endif