#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Core::Input
{
	class Input
	{
	public:
		virtual void compute(GLFWwindow* window) = 0;
	};
}