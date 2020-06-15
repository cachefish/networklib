#ifndef _SOCKET_H_
#define _SOCKET_H_

#include"Noncopyable.h"

namespace cf
{
    class InetAddress;
    class Socket:Noncopyable
    {
        public:
            Socket(int sockfd);
            Socket();
            ~Socket();

            void shutdownWrite();
            int fd()const{return sockfd_;}

            void nonblock();

            static InetAddress getLocalAddr(int sockfd);
            static InetAddress getPeerAddr(int sockfd);
        private:
            int sockfd_;
    };


} // namespace cf





#endif