#pragma once

#include "component.hpp"
#include "game_master.hpp"

namespace Gameplay
{
	class WinScreen : public Engine::Component
	{
	public:
		Engine::GameObject* buttons[3] = { nullptr };

		WinScreen(Engine::GameObject& gameObject);

		void start() override;
		void drawImGui() override;

		void showWinScreen(bool isActive);

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}