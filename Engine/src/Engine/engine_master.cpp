#include "engine_master.hpp"

#include "inputs_manager.hpp"
#include "application.hpp"
#include "physic_manager.hpp"
#include "render_manager.hpp"
#include "sound_manager.hpp"
#include "debug.hpp"
#include "graph.hpp"
#include "time.hpp"

namespace Core::Engine
{
	EngineMaster::EngineMaster()
	{
		Core::Debug::Log::info("Creating the Engine");
	}

	EngineMaster::~EngineMaster()
	{
		Core::Debug::Log::info("Destroying the Engine");

		// Kill managers
		Graph::kill();
		Physics::PhysicManager::kill();
		LowRenderer::RenderManager::kill();
		SoundManager::kill();
	}

	void EngineMaster::init()
	{
		EngineMaster* EM = instance();

		Core::Input::InputManager::addButton("Edit Toggle", GLFW_KEY_ESCAPE);

		Engine::SoundManager::init();
	}

	void EngineMaster::toggleEditMode()
	{
		EngineMaster* EM = instance();

		if (EM->lockEdit) return;

		EM->editMode = !EM->editMode;

		Core::Application::setCursor(EM->editMode);
	}

	void EngineMaster::lockEditMode(bool isLock)
	{
		EngineMaster* EM = instance();

		EM->lockEdit = isLock;
		EM->editMode = !isLock;
		Core::Application::setCursor(!isLock);
	}

	void EngineMaster::update()
	{
		EngineMaster* EM = instance();

		if (Core::Input::InputManager::getButtonDown("Edit Toggle"))
			EM->toggleEditMode();

		Graph::checkRequests();
		Graph::draw();

		if (EM->editMode)
			Graph::drawImGui();
		else 
			Graph::update();

		Graph::clean();
	}
}	