#include "thread_pool.hpp"

#include <string>
#include <sstream>

#include "debug.hpp"

ThreadPool::ThreadPool()
{
	Core::Debug::Log::info("Creating the Thread Pool");
}

ThreadPool::~ThreadPool()
{
	/*finish = true;

	for (std::thread& thread : threads)
		thread.join();*/
}

void ThreadPool::end()
{
	ThreadPool* TP = instance();

	TP->finish = true;

	for (std::thread& thread : TP->threads)
		thread.join();
}

void ThreadPool::init()
{
	ThreadPool* TP = ThreadPool::instance();

	if (TP->initialized)
	{
		Core::Debug::Log::error("Thread Pool is already initialized");
		return;
	}

	TP->initialized = true;

	Core::Debug::Log::info("Initialize the Thread Pool");

	TP->maxNumThreads = std::thread::hardware_concurrency();

	// Create permanent threads - Workers
	for (unsigned int i = 0; i < TP->maxNumThreads; i++)
		TP->threads.push_back(std::thread(&ThreadPool::threadRoutine, TP));
}

bool ThreadPool::running()
{
	return instance()->count.load() > 0;
}

bool ThreadPool::allowKill()
{
	ThreadPool* TP = instance();
	return TP->countEnd.load() == TP->maxNumThreads;
}

void ThreadPool::threadRoutine()
{
	while (!finish)
	{
		// Routine, take a task and make it, or wait for another task
		while (taskManager.test_and_set() && !finish)
			std::this_thread::sleep_for(std::chrono::milliseconds(20));

		if (tasks.empty())
		{
			taskManager.clear();
			continue;
		}

		std::function<void()> task = tasks.front();
		tasks.pop();
		taskManager.clear();

		task();

		count.fetch_sub(1);
	}

	std::stringstream ss;
	ss << std::this_thread::get_id();

	Core::Debug::Log::info("Thread " + ss.str() + " has finished");

	countEnd.fetch_add(1);
}