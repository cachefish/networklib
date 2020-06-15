#ifndef __WD_THREAD_H__
#define __WD_THREAD_H__

#include "Noncopyable.h"

#include <pthread.h>
#include <functional>
using std::function;

namespace cf
{


class Thread
: Noncopyable 
{
	using ThreadCallback = function<void()>;
public:
	Thread(ThreadCallback && cb);
	~Thread();

	void start();
	void join();
	
	bool isRunning() const {	return _isRunning;	}

private:
	static void * threadFunc(void *);//采用静态成员函数，就是为了去掉this的影响
private:
	pthread_t _pthid;
	bool _isRunning;
	ThreadCallback _cb;
};
}//end of namespace cf
 

#endif
