#ifndef _EPOLLPOLLER_H
#define _EPOOLPOLLER_H

#include"Noncopyable.h"
#include"TcpConnection.h"
#include<sys/epoll.h>
#include<vector>
#include<map>
#include<functional>

namespace cf
{   
    class Acceptor;
    class EpollPoller:cf::Noncopyable
    {
        public:
            typedef TcpConnection::TcpConnectionCallback EpollCallback;
            typedef std::function<void()> Functor;

            EpollPoller(Acceptor&acceptor);
            ~EpollPoller();

            void loop();
            void unloop();
            void doPendingFunctors();

            void setConnectionCallback(EpollCallback cb);
            void setMessageCallback(EpollCallback cb);
            void setCloseCallback(EpollCallback cb);
        private:
            void waitEpollfd();
            void handleConnection();
            void handleMessage(int peerfd);

        private:
            Acceptor&acceptor_;
            int epollfd_;
            int listenfd_;
            bool isLooping_;

            typedef std::vector<struct epoll_event> EventList;
            EventList eventsList_;

            typedef std::map<int,TcpConnectionPtr> ConnectionMap;
            ConnectionMap connMap_;

            EpollCallback onConnectionCb_;
            EpollCallback onMessageCb_;
            EpollCallback onCloseCb_;
    };

}


#endif