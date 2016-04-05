#ifndef THREADPOOL_H
#define THREADPOOL_H
/////////////////////////////////////////////////////////////////////
//  ThreadPool.h - Analyzes C++ language constructs                //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Application:   ThreadPool for CSE687 Pr3                       //
//  Author:        Yichen Jiang                                    //
//  Reference:     Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module is used for creating a threadpool, so when someone is using
the threadpool, we will create three threads for them.

Public Interface:
=================
BlockingQueue<WorkItem<Result>*>& getWorkingQueue()   //this is used to get working queue of blocking queue
void start();                                         //this is used to get threadpool start
void working(WorkItem<Result>* pWi);                  //this is used to get the working item
size_t getThreadCount();                              //this is used to know how many threads can be in the threadpool
void suspend();                                       //this is used to wait for joinging the threads
Result result();                                      //this is used to return result queue
~ThreadPool();                                        // destruct threadpool


Build Process:
==============
Required files
- Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp, ThreadPool.h, ThreadPool.cpp
  Utility.h, Utility.cpp
Build commands
- devenv ThreadPool.sln

Maintenance History:
====================
ver 1.0 : 4 Apr 16
- first release

*/

#include <thread>
#include <functional>
#include <conio.h>
#include "../BlockQueue/Cpp11-BlockingQueue.h"

template<typename Result>
using WorkItem = std::function<Result()>;

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result>
class ThreadPool
{
public:
	BlockingQueue<WorkItem<Result>*>& getWorkingQueue() { return workingQueue; }
	void start();
	void working(WorkItem<Result>* pWi);
	size_t getThreadCount();
	Result result();
	void joining();
	~ThreadPool();
private:
	BlockingQueue<WorkItem<Result>*> workingQueue;
	std::thread* pThread;
	std::vector<std::thread*> pThreadPool;
	BlockingQueue<Result> resultsQueue;
};
//this is used for starting the threadpool-------------------

template<typename Result>
void ThreadPool<Result>::start()
{
	for (size_t i = 0; i < 3; i++)
	{
		std::function<void()> threadProc =
			[&]() {
			while (true)
			{
				WorkItem<Result>* pWi = workingQueue.deQ();
				if (pWi == nullptr)
				{
					std::cout << "\n  shutting down work item processing, thread id: " << std::this_thread::get_id();
					return;
				}
				Result result = (*pWi)();
				resultsQueue.enQ(result);
			}
		};
		pThread = new std::thread(threadProc);
		pThreadPool.push_back(pThread);
	}

}
//join child thread ------------------------------------------------

template<typename Result>
void ThreadPool<Result>::joining()
{
	size_t len = pThreadPool.size();
	for (size_t i = 0; i < len; i++)
	{
		pThreadPool[i]->join();
	}
}
// enqueue work item --------------------------------------------------

template<typename Result>
void ThreadPool<Result>::working(WorkItem<Result>* pWi)
{
	workingQueue.enQ(pWi);
}
//used to get know how many threads can be in the threadpool--------------------------

template<typename Result>
inline size_t ThreadPool<Result>::getThreadCount()
{
	size_t len = pThreadPool.size();
	return len;
}
//this is used for getting the result of the resultqueue---------------------------------
template<typename Result>
Result ThreadPool<Result>::result()
{
	return resultsQueue.deQ();
}

//this is used for destruct threadpool------------------------------------------------

template<typename Result>
ThreadPool<Result>::~ThreadPool()
{
	//delete _pThreadPool;
}

#endif