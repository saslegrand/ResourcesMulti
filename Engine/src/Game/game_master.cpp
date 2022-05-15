#include "game_master.hpp"

#include <imgui.h>

#include "inputs_manager.hpp"
#include "graph.hpp"
#include "time.hpp"

#include "pause_screen.hpp"
#include "enemy_life.hpp"
#include "lose_screen.hpp"
#include "win_screen.hpp"

namespace Gameplay
{
	GameMaster::GameMaster(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<GameMaster>(this))
	{

	}

	void GameMaster::start()
	{
		Core::Engine::Graph::setCursorState(false);
		pauseScreen = Core::Engine::Graph::findGameObjectWithName("PauseScreen")->getComponent<PauseScreen>();
		winScreen = Core::Engine::Graph::findGameObjectWithName("WinScreen")->getComponent<WinScreen>();
		loseScreen = Core::Engine::Graph::findGameObjectWithName("LoseScreen")->getComponent<LoseScreen>();
	}

	void GameMaster::update()
	{
		if (Core::Input::InputManager::getButtonDown("Pause"))
		{
			isPaused = true;
			Core::TimeManager::setTimeScale(0.f);
			pauseScreen->showPauseScreen(true);
		}

		if (Core::Input::InputManager::getButtonDown("Save"))
		{
			Core::Engine::Graph::saveCurrentScene();
			Core::Debug::Log::info("Scene has been saved");
		}
	}

	void GameMaster::removeEnemy()
	{
		enemyCount--;

		if (enemyCount == 0)
		{
			isPaused = true;
			Core::TimeManager::setTimeScale(0.f);
			winScreen->showWinScreen(true);
		}
	}

	void GameMaster::removePlayer()
	{
		isPaused = true;
		Core::TimeManager::setTimeScale(0.f);
		loseScreen->showLoseScreen(true);
	}

	void GameMaster::drawImGui()
	{
		if (ImGui::TreeNode("GameMaster"))
		{
			ImGui::Text("F5 to save the scene");
			ImGui::Text("ESCAPE tu return main menu");
			std::string str = "enemyCount = " + std::to_string(enemyCount);
			const char* cstr = str.c_str();
			ImGui::Text(cstr);

			ImGui::TreePop();
		}
	}

	std::string GameMaster::toString() const
	{
		return "COMP GAMEMASTER";
	}

	void GameMaster::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<GameMaster>())
			gameObject.addComponent<GameMaster>();
	}
}