#ifndef _SOCKETIO_H__
#define _SCOKETIO_H__

#include<stdio.h>

namespace cf
{
    class SocketIO
    {

    public:
        SocketIO(int sockfd);

        size_t readn(char*buf,size_t count);
        size_t writen(const char*buf,size_t count);
        size_t readline(char*buf,size_t maxlen);
    private:
        size_t recv_peek(char*buf,size_t count);
    private:   
        int sockfd_;

    };

}

#endif