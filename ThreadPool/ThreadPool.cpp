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
* A threadpool processes work items equeued by main thread
*/

#include <string>
#include <iostream>
#include <conio.h>
#include "ThreadPool.h"
#include "../Utilities/Utilities.h"

//----< demonstrate ThreadPool class >----------------------------


#ifdef TEST_THREADPOOL
using Util = Utilities::StringHelper;
using WorkResult = std::string;

int main()
{
	Util::Title("Enqueued Work Items");

	std::cout << "\n  main thread id = " << std::this_thread::get_id();

	ThreadPool<WorkResult> processor;
	processor.start();

	// define 1st work item

	WorkItem<WorkResult> wi1 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "Hello from wi1";
	};

	processor.working(&wi1);

	// define 2nd work item

	WorkItem<WorkResult> wi2 = []()
	{
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		size_t sum = 0;
		for (size_t i = 0; i < 100000; ++i)
			sum += i;
		return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
	};

	processor.working(&wi2);

	// the following calls to result() block until results are enqueued

	// define 3rd work item

	WorkItem<WorkResult> wi3 = []() {
		std::cout << "\n  working on thread " << std::this_thread::get_id();
		return "Hello from wi3";
	};

	processor.working(&wi3);

	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	processor.working(nullptr);
	processor.working(nullptr);
	processor.working(nullptr);


	// wait for child thread to complete

	processor.joining();
	std::cout << "\n\n";
	system("pause");
}


#endif