#ifndef _SOCKET_H__
#define _SOCKET_H__

#include "Noncopyable.h"

namespace cf
{
    class InetAddress;

    class Socket:public cf::Noncopyable
    {
        public:
            Socket(int sockfd);
            Socket();
            ~Socket();

            void nonblock();
            void shutdowmWrite();
            int fd()const{return sockfd_;}

            static InetAddress getLocalAddr(int sockfd);
            static InetAddress getPeerAddr(int sockfd);

        private:
            int sockfd_;

    };

}


#endif