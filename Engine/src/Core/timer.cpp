#include "timer.hpp"

#include "time.hpp"

namespace Core
{
	void Timer::update()
	{
		if (isPass)
			return;

		currentTime -= TimeManager::getDeltaTime();
		isPass = currentTime <= 0.f;
	}

	void Timer::setDelay(float myTime)
	{
		delay = myTime;
		reset();
	}

	bool Timer::timerOn()
	{
		return isPass;
	}

	void Timer::reset()
	{
		isPass = false;
		currentTime = delay;
	}
}