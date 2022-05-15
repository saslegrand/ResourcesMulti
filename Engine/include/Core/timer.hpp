#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "button.hpp"
#include "maths.hpp"

namespace Core
{
	class Timer
	{
	private:
		float currentTime = 0.f;
		float delay = 0.f;
		bool isPass = false;
	public:
		bool timerOn();
		void setDelay(float myTime);
		void reset();
		void update();
	};
}