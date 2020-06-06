#include"SocketIO.h"
#include"SocketUtil.h"
#include"InetAddress.h"
#include"Acceptor.h"
#include"TcpConnection.h"

#include<string>
#include<iostream>
using std::string;
using std::cout;
using std::cin;
using std::endl;

int main(int argc,char*argv[])
{
    wd::InetAddress addr("192.168.95.142",8888);
    wd::Acceptor acceptor(wd::createSocketfd(),addr);
    acceptor.ready();

    int peerfd = acceptor.accept();
    wd::TcpConnection conn(peerfd);
    conn.send("welcom to chat server!");
    while(1)
    {
        string msg = conn.receive();
        cout<<"server receve:"<<msg<<endl;
        conn.send(msg);
    }
    return 0;
}

