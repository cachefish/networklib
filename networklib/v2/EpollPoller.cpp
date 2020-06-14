#include"EpollPoller.h"
#include"SocketUtil.h"
#include"Acceptor.h"
#include<assert.h>

namespace wd
{
EpollPoller::EpollPoller(Acceptor &acceptor)
:acceptor_(acceptor)
,epollfd_(createEpollFd())
,listenfd_(acceptor_.fd())
,isLooping_(false)
,eventList_(1024)
{
    addEpollFdRead(epollfd_,listenfd_);
}

EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}

void EpollPoller::loop()
{
    isLooping_ = true;
    while (isLooping_)
    {
        /* code */
        waitEpollfd();
    }
    
}

void EpollPoller::unloop()
{
    if(isLooping_){
        isLooping_ = false;
    }
}
void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	onConnectionCb_ = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
	onMessageCb_ = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
	onCloseCb_ = cb;
}

void EpollPoller::waitEpollfd()
{
    int nready;
    do{
        nready = ::epoll_wait(epollfd_,&(*eventList_.begin()),eventList_.size(),5000);
    }while(nready == -1&&errno == EINTR);
    
    if(nready == -1)
    {
        perror("epoll_wait error");
        exit(EXIT_FAILURE);
    }else if(nready ==0) 
    {
        printf("epoll_wait ttimeout");
    }
    else{
        if(nready == static_cast<int>(eventList_.size()))
        {
            eventList_.resize(eventList_.size()*2);
        }
        //遍历每一个激活的文件描述符
        for(int idx =0;idx!= nready;++idx)
        {
            if(eventList_[idx].data.fd == listenfd_)
            {
                if(eventList_[idx].events &&EPOLLIN)
                {
                    handleConnection();
                }
            }else{
                if(eventList_[idx].events&EPOLLIN){
                    handleMessage(eventList_[idx].data.fd);
                }
            }
        }
    }
}

//创建连接
void EpollPoller::handleConnection()
{
    int peerfd = acceptor_.accept();
    addEpollFdRead(epollfd_,peerfd);

    TcpConnectionPtr conn(new TcpConnection(peerfd));
    
    conn->send("welcome to server\n");
    conn->setConnectionCallback(onConnectionCb_);
    conn->setMessageCallback(onMessageCb_);
    conn->setCloseCallback(onCloseCb_);

    std::pair<ConnectionMap::iterator,bool> ret;
    ret = connMap_.insert(std::make_pair(peerfd,conn));
    assert(ret.second == true);
    conn->handleConnectionCallback();

}

void EpollPoller::handleMessage(int peerfd)
{
    bool isClosed = isConnectionClosed(peerfd);
    ConnectionMap::iterator it = connMap_.find(peerfd);
    assert(it!=connMap_.end());

    if(isClosed){
        it->second->handleCloseCallback();
        delEpollReadFd(epollfd_,peerfd);
        connMap_.erase(it);
    }else{
        it->second->handleMessageCallback();
    }
}



}



