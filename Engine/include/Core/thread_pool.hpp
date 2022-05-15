#pragma once

#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <functional>// par la aussi
#include <iostream>

#include "singleton.hpp"

class ThreadPool final : public Singleton<ThreadPool>
{
	friend Singleton<ThreadPool>;
public:
	ThreadPool();
	~ThreadPool();

	static void init();
	static void end();

	template <typename Task>
	static void addTask(Task task)
	{
		ThreadPool* TP = instance();

		// Lock free - Avoid multiple call at the same time
		while (TP->taskManager.test_and_set());

		TP->count.fetch_add(1);
		TP->tasks.push(std::function<void()>(task));

		// Notify at least one thread waiting 
		// taskManager.notify_one();
		TP->taskManager.clear();
	}

	static bool running();
	static bool allowKill();

	//static ThreadPool* getInstance() { return instance(); } rayane est aussi passe par la

private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;

	std::atomic_flag taskManager = ATOMIC_FLAG_INIT;

	unsigned int maxNumThreads = 0;

	void threadRoutine();

	bool initialized = false;
	bool finish = false;

	std::atomic<int> count{ 0 };
	std::atomic<unsigned int> countEnd{ 0 };
};