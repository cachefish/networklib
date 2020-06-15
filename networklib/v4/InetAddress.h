#ifndef _INETADDRESS_H__
#define _INETADDRESS_H__

#include<netinet/in.h>
#include<string>

namespace cf
{
    class InetAddress
    {
        public:
            InetAddress(short port);
            InetAddress(const char*plp,short port);
            InetAddress(const struct sockaddr_in&addr);

            const struct sockaddr_in*getSockAddrPtr()const;

            std::string ip()const;
            unsigned short port()const;
        private:
            struct sockaddr_in addr_;
    };
} // namespace cf






#endif