#pragma once
#include "singleton.hpp"

namespace Core
{
	class TimeManager final : Singleton<TimeManager>
	{
		friend class Singleton<TimeManager>;
		friend class Application;
		friend class Graph;

	private:
		float time = 0.f;
		float lastTime = 0.f;
		float deltaTime = 1.f / 60.f;
		float fixedDeltaTime = 1.f / 60.f;
		float unscaledDeltaTime = 0.f;

		TimeManager();
		~TimeManager();

		static void computeTime();

	public:
		float timeScale = 1.f;

		static void resetTime();
		static float getTime();
		static float getDeltaTime();
		static float getFixedDeltaTime();
		static float getUnscaledDeltaTime();

		static void  setTimeScale(float value);
	};
}