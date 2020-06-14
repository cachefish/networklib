#include"TcpServer.h"
#include<iostream>
using std::cout;
using std::endl;

void onConnection(const cf::TcpConnectionPtr &conn)
{   
    cout<<conn->toString()<<endl;
    conn->send("hello,welcome to chat server\r\n");
}
void onMessage(const cf::TcpConnectionPtr&conn)
{
    std::string s(conn->receive());
    
    conn->send(s);
}

void onClose(const cf::TcpConnectionPtr&conn)
{
    printf("%s close\n",conn->toString().c_str() );
}

int main(int argc,const char *argv[])
{
    cf::TcpServer tcpserver("192.168.1.108",8888);
    tcpserver.setConnectionCallback(&onConnection);
    tcpserver.setMessageCallback(&onMessage);
    tcpserver.setCloseCallback(&onClose);

    tcpserver.start();
    return 0;
}



