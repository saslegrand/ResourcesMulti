#pragma once

#include "component.hpp"
#include "game_master.hpp"

namespace Gameplay
{
	class PauseScreen : public Engine::Component
	{
	private:
		std::shared_ptr<GameMaster> gameMaster;

	public:
		Engine::GameObject* buttons[3] = {nullptr};

		PauseScreen(Engine::GameObject& gameObject);

		void start() override;
		void drawImGui() override;

		void showPauseScreen(bool isActive);

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}