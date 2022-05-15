#pragma once

#include <atomic>

template <class T>
class Singleton
{
private:
	static std::atomic_flag isInitialized;
protected:
	Singleton() = default;

	static T* currentInstance;

	[[nodiscard]] static T* instance()
	{
		if (!isInitialized.test_and_set())
		{
			if (!currentInstance)
				currentInstance = new T();
		}
		
		return currentInstance;
	}

public:
	static void kill()
	{
		delete currentInstance;
		currentInstance = nullptr;

		isInitialized.clear();
	}

	Singleton(T&) = delete;

	void operator=(const T&) = delete;
};

template <class T> T* Singleton<T>::currentInstance = nullptr;
template <class T> std::atomic_flag Singleton<T>::isInitialized = ATOMIC_FLAG_INIT;