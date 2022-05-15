#pragma once

#include "component.hpp"
#include "game_master.hpp"

namespace Gameplay
{
	class LoseScreen : public Engine::Component
	{	
	public:
		Engine::GameObject* buttons[3] = { nullptr };
	
		LoseScreen(Engine::GameObject& gameObject);
	
		void start() override;
		void drawImGui() override;
	
		void showLoseScreen(bool isActive);
	
		std::string toString() const override;
	
		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}