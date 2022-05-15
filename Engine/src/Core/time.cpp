#include "time.hpp"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <algorithm>

#include "debug.hpp"

namespace Core
{
	TimeManager::TimeManager()
	{
		Debug::Log::info("Creating the Time Manager");
	}

	TimeManager::~TimeManager()
	{
		Debug::Log::info("Destroying the Time Manager");
	}

	void TimeManager::computeTime()
	{
		TimeManager* TM = instance();

		// Update the Application Time
		TM->time = (float)glfwGetTime();

		TM->unscaledDeltaTime = TM->time - TM->lastTime;
		TM->deltaTime = TM->unscaledDeltaTime * TM->timeScale;

		TM->lastTime = TM->time;
	}

	void TimeManager::resetTime()
	{
		computeTime();
	}

	float TimeManager::getTime()
	{
		return instance()->time;
	}

	float TimeManager::getDeltaTime()
	{
		return instance()->deltaTime;
	}

	float TimeManager::getFixedDeltaTime()
	{
		return instance()->fixedDeltaTime;
	}

	float TimeManager::getUnscaledDeltaTime()
	{
		return instance()->unscaledDeltaTime;
	}

	void TimeManager::setTimeScale(float value)
	{
		instance()->timeScale = std::max(0.f, value);
	}
}