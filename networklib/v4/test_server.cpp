#include"TcpServer.h"
#include"Threadpool.h"
#include<iostream>
using std::cout;
using std::endl;

class Task
{
    public:
        Task(const string&query,const cf::TcpConnectionPtr&conn)
        :_query(query)
        ,_conn(conn)
        {

        }
        void process()
        {
            cout<<">task is processing"<<endl;
            _conn->sendInLoop(_query);
        }
    private:
        string  _query;
        cf::TcpConnectionPtr  _conn;
};

cf::Threadpool *g_threadpool = NULL;
void onConnection(const cf::TcpConnectionPtr&conn)
{
    cout<<conn->toString()<<endl;
    conn->send("hello,welcome to chat Server\r\n");
}
//运行在io线程
void onMessage(const cf::TcpConnectionPtr&conn)
{
    std::string s(conn->receive());
    Task task(s,conn);
    g_threadpool->addTask(std::bind(&Task::process,task));
    cout<<">add task to threadpool"<<endl;
}
void onClose(const cf::TcpConnectionPtr &conn)
{
    printf("%s close\n", conn->toString().c_str());
}

int main(int argc, char const *argv[])
{
	cf::Threadpool threadpool(4, 10);
	g_threadpool = &threadpool;
	threadpool.start();

	cf::TcpServer tcpserver("192.168.1.108", 8888);
	tcpserver.setConnectionCallback(&onConnection);
	tcpserver.setMessageCallback(&onMessage);
	tcpserver.setCloseCallback(&onClose);

	tcpserver.start();

    return 0;
}