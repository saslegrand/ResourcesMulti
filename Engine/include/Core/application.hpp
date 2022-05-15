#pragma once

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "singleton.hpp"
#include "maths.hpp"

namespace Core
{
	class Application final : public Singleton<Application>
	{
		friend class Singleton<Application>;

	private:
		bool initialized = false;
		GLFWwindow* window = nullptr;

		Core::Maths::vec2 windowSize;
		float aspect = 0.f;

		Application();
		~Application();

		[[nodiscard]] static GLFWwindow* createWindow(unsigned int screenWidth, unsigned int screenHeight, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		void setImGuiColorsEditor();
		bool cantClose();

	public:
		static void init(unsigned int screenWidth, unsigned int screenHeight, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
		static void update();
		static float getAspect();
		static void setCursor(bool isCursorFree);
		static void updateWindowSize(int width, int height);

		static Core::Maths::vec2 getWindowSize();

		static void closeApplication();
	};
}