#include <string>
#include <iostream>
#include <crtdbg.h>

#include "application.hpp"

#include "debug.hpp"

const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 900;

int main()
{
	// Check for leak
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		Core::Application::init(SCR_WIDTH, SCR_HEIGHT, "Engine");

		Core::Application::update();
	}
	catch (const char* msg)
	{
		Core::Debug::Log::assertion(msg);
	}
	catch (const std::string& msg)
	{
		Core::Debug::Log::assertion(msg);
	}
	catch (const std::exception& assertion)
	{
		Core::Debug::Log::assertion(assertion.what());
	}

	Core::Application::kill();

	return 0;
}