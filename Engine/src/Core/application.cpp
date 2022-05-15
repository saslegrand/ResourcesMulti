#include "application.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "engine_master.hpp"
#include "thread_pool.hpp"
#include "graph.hpp"
#include "debug.hpp"
#include "time.hpp"

// glfw - Whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// Modify viewport size after window resize
	glViewport(0, 0, width, height);
}

namespace Core
{
	Application::Application()
	{
		Core::Debug::Log::info("Creating the Application");
	}

	Application::~Application()
	{
		ThreadPool::end();

		while (!ThreadPool::allowKill());

		ThreadPool::kill();

		Resources::ResourcesManager::kill();

		Debug::Log::info("Destroying the Application");

		Input::InputManager::kill();
		Debug::Assertion::kill();
		Engine::EngineMaster::kill();

		TimeManager::kill();


		Debug::Log::kill();

		// Destroy ImGui context
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Destroy glfw window
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	GLFWwindow* Application::createWindow(unsigned int screenWidth, unsigned int screenHeight, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	{
		// Check if the Application instance is already initialized
		if (instance()->initialized)
			return nullptr;

		// glfw - Initialize and configuration
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create window
		Debug::Log::info("Creating the window");
		GLFWwindow* newWindow = glfwCreateWindow(screenWidth, screenHeight, title, monitor, share);

		// If the window could not be created, exit the program
		Debug::Assertion::out(newWindow, "Failed to create GLFW window");

		// Initialize glfw context
		glfwMakeContextCurrent(newWindow);
		glfwSetFramebufferSizeCallback(newWindow, framebufferSizeCallback);

		// glad - Load all OpenGL function pointers, if it fails, assert
		Debug::Assertion::out(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");

		// ImGui - Initialization
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(newWindow, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		return newWindow;
	}

	void windowResizeCallback(GLFWwindow* window, int width, int height)
	{
		Application::updateWindowSize(width, height);
	}

	void test(int index)
	{
		Core::Debug::Log::info("Bijour je suis une tache et j'ai l'index " + std::to_string(index) + " ");
	}

	void Application::init(unsigned int screenWidth, unsigned int screenHeight, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	{
		Application* AP = instance();

		// Check if the Application is already initialized
		if (AP->initialized)
		{
			Debug::Log::error("The Application is already initialized");
			return;
		}

		// Return the Window created by GLFW
		AP->window = createWindow(screenWidth, screenHeight, title, monitor, share);
		AP->initialized = true;
 		Debug::Log::info("Application initialized");

		glfwSetWindowSizeCallback(AP->window, windowResizeCallback);

		int width, height;
		glfwGetWindowSize(AP->window, &width, &height);
		updateWindowSize(width, height);

		// Init Managers
		Resources::ResourcesManager::init();

		ThreadPool::init();
		Core::Engine::Graph::init();

		Input::InputManager::init(AP->window);

		Engine::EngineMaster::init();

		LowRenderer::RenderManager::init();

		AP->setImGuiColorsEditor();
	}

	bool Application::cantClose()
	{
		return !glfwWindowShouldClose(window);
	}

	void Application::update()
	{
		Application* AP = instance();

		glfwSetInputMode(AP->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Precompute time
		// Avoid big deltatime while loading resources
		Core::TimeManager::computeTime();

		// Loop while the game is running
		while (AP->cantClose())
		{
			//Update ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//Compute managers
			TimeManager::computeTime();
			Input::InputManager::compute();

			//Update the Engine
			Engine::EngineMaster::update();

			//Render ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			//glfw - Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwSwapBuffers(AP->window);
			glfwPollEvents();
		}
	}

	void Application::closeApplication()
	{
		// Tell to glfw to close the window 
		glfwSetWindowShouldClose(instance()->window, true);
	}

	float Application::getAspect()
	{
		// Return the aspect of the Window
		return instance()->aspect;
	}

	void Application::setCursor(bool isCursorFree)
	{
		Application* AP = instance();

		// Toggle the cursor visibility
		if (!isCursorFree)
		{
			glfwSetInputMode(AP->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			return;
		}

		glfwSetInputMode(AP->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	Core::Maths::vec2 Application::getWindowSize()
	{
		return instance()->windowSize;
	}

	void Application::updateWindowSize(int width, int height)
	{
		Application* AP = instance();

		AP->windowSize = { (float)width, (float)height };
		AP->aspect = AP->windowSize.x / AP->windowSize.y;
	}

	void Application::setImGuiColorsEditor()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.61f, 0.52f, 0.16f, 0.68f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.10f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.23f, 0.12f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.50f, 0.42f, 0.26f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.72f, 0.60f, 0.37f, 0.54f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.76f, 0.67f, 0.50f, 0.54f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.61f, 0.52f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.61f, 0.52f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.54f, 0.41f, 0.16f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.63f, 0.44f, 0.10f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.67f, 0.44f, 0.16f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.86f, 0.55f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.55f, 0.25f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.52f, 0.33f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.54f, 0.31f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.67f, 0.44f, 0.16f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.76f, 0.50f, 0.19f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.63f, 0.29f, 0.40f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.94f, 0.69f, 0.39f, 0.40f);
		colors[ImGuiCol_Header] = ImVec4(0.76f, 0.58f, 0.19f, 0.40f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.87f, 0.68f, 0.23f, 0.31f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.95f, 0.78f, 0.39f, 0.40f);
		colors[ImGuiCol_Separator] = ImVec4(0.42f, 0.24f, 0.13f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.42f, 0.24f, 0.13f, 0.50f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.27f, 0.17f, 0.50f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.61f, 0.52f, 0.16f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.73f, 0.63f, 0.22f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.73f, 0.63f, 0.22f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.92f, 0.77f, 0.37f, 0.40f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.68f, 0.48f, 0.18f, 0.40f);
		colors[ImGuiCol_TabActive] = ImVec4(0.50f, 0.37f, 0.21f, 0.40f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.77f, 0.37f, 0.40f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.50f, 0.37f, 0.22f, 0.40f);
	}
}