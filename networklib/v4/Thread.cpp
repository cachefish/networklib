#include "Thread.h"
#include <iostream>

using std::cout;
using std::endl;
using namespace cf; 


//右值引用本身到底是左值还是右值，取决于其是否有名字
Thread::Thread(ThreadCallback && cb)
: _pthid(0)
, _isRunning(false)
, _cb(std::move(cb))
{
	cout << "Thread(cb)" << endl;
}

void Thread::start()
{
	pthread_create(&_pthid, NULL, threadFunc, this);
	_isRunning = true;
}

void * Thread::threadFunc(void * arg)
{
	Thread * pthread = static_cast<Thread*>(arg);
	if(pthread)
		pthread->_cb();// 执行任务
	return NULL;
}

void Thread::join()
{
	pthread_join(_pthid, NULL);
	_isRunning = false;
}

Thread::~Thread()
{
	if(_isRunning)	
	{
		pthread_detach(_pthid);// 将运行的线程交给系统进行托管
		_isRunning = false;
	}
	cout << "~Thread()" << endl;
}



