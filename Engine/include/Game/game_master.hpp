#pragma once

#include "component.hpp"

namespace Gameplay
{
	class PauseScreen;
	class WinScreen;
	class LoseScreen;

	class GameMaster : public Engine::Component
	{
	private:
		std::shared_ptr<PauseScreen> pauseScreen;
		std::shared_ptr<WinScreen> winScreen;
		std::shared_ptr<LoseScreen> loseScreen;

	public:
		GameMaster(Engine::GameObject& gameObject);

		bool isPaused = false;
		int enemyCount = 0;

		void start() override;
		void update() override;
		void drawImGui() override;
		void removeEnemy();
		void removePlayer();

		std::string toString() const override;
		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}