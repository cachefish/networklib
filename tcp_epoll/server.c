#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define ERR_EXIT(m)\
    do{ \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

    #define MAXEVENTS 2048
    
    void do_accept(int sockfd);

    int main(int argc,char*argv[])
    {
        int listenfd = socket(PF_INET,SOCK_STREAM,0);
        if(listenfd == -1){
            ERR_EXIT("socket");
        }
        //地址复用
        int on =1;
        if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0){
            ERR_EXIT(""setsockopt);
        }

        struct sockaddr_in addr;
        memset(&addr,0,sizeof(addr));
        addr.sin_addr.s_addr = inet_addr("192.168.95.142");
        addr.sin_family = AF_INET;
        addr.sin_port=htons(8888);
        if(bind(listenfd,(struct sockaddr*)&addr,sizeof(addr))==-1){
            ERR_EXIT("listen");
        }

    //创建epoll事件循环
    int efd = epoll_create1(0);
    if(-1==efd){
        perror(epool_ceeate error);
        exit(EXIT_FAILURE);
    }

    struct epoll_event evt_listen;
    evt_listen.data.fd = listenfd;
    evt_listen.events=EPOLLIN;
    int ret = epoll_ctl(efd,EPOLL_CTL_ADD,listenfd,&evt_listen);
    if(-1==ret){
        ERR_EXIT("epoll_ctl");
    }

    struct epoll_event*events = calloc(MAXEVENTS,sizeof(struct epoll_event));

    while(1)
    {
        int idx;
        int nready = epoll_wait(efd,events,MAXEVENTS,-1);
        for(idx=0;idx!=nready;++idx){
            int fd = events[idx].data.fd;
            if(fd == listenfd){
                //新客户端到来
                if(events[idx].events&EPOLLIN)
                {
                    int peerfd = accept(fd,NULL,NULL);
                    if(-1==peerfd)
                    {
                        ERR_EXIT("accept");
                    }
                    struct epoll_event ev;
                    ev.data.fd = peerfd;
                    ev.events = EPOLLIN;
                    ret = epoll_ctl(efd,EPOLL_CTL_ADD,peerfd,&ev);
                    if(-1==ret)
                    {
                        ERR_EXIT("epoll_ctl_add");
                    
                    }
                }
                //可以使用回调函数在建立新连接后打印一些信息
                //onConnection();
            }else{
                //与客户端建立的fd
                if(events[idx].events&EPOLLIN){
                    char recvbuf[1024]={0};
                    int nread =read(fd,recvbuf,sizeof(recvbuf));
                    if(0==nread){
                        //链接断开之后的处理
                        struct epoll_event ev;
                        ev.data.fd = fd;
                        ret = epoll_ctl(efd,EPOLL_CTL_DEL,fd,&ev);
                        if(-1==ret){
                            ERR_EXIT("epoll_ctl_del");
                        }
                        close(fd);
                        //onClose();
                        continue;
                    }
                    //对客户端数据的处理
                    //onMessage();
                #if 1
                    printf("recv msg :%s\n",recvbuf)
                    int nwrite=write(fd,recvbuf,strlen(recvbuf));
                    if(-1==nwrite)
                    {
                        ERR_EXIT("send data err");
                    }
                #endif
                }
            }
        }
    }
    free(events);
close(listenfd);
return 0;
}
#if 0
void do_service(int sockfd)
{
    char recvbuf[1024] = {0};
    while(1)
    {
        int nread = read(sockfd, recvbuf, sizeof recvbuf);
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("read");
        }
        else if(nread == 0)
        {
            printf("close ...\n");
            exit(EXIT_SUCCESS);
        }


        write(sockfd, recvbuf, strlen(recvbuf));
        memset(recvbuf, 0, sizeof recvbuf);
    }
}
#endif