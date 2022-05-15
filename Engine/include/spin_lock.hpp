#pragma once

#include <atomic>

struct SpinLock
{
	std::atomic_flag lock = ATOMIC_FLAG_INIT;

	void spin()
	{
		while (!lock.test_and_set())
		{
			// Do other job
		}

		// Do main job

		lock.clear();
	}
};
