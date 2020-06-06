
#ifndef __SOCKET_H
#define __SOCKET_H

#include "Noncopyable.h"

namespace wd
{
class InetAddress;
class Socket : Noncopyable
{
public:
	explicit Socket(int sockfd);
	Socket();
	~Socket();

	void shutdownWrite();
	int fd()const{	return sockfd_;	}

	void nonblock();

	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);
private:
	int sockfd_;
};
}// end of namespace wd

#endif
