#pragma once
#include <string>

#include "entity_life.hpp"
#include "game_master.hpp"

namespace Gameplay
{
	class PlayerLife : public EntityLife
	{
	private:
		void kill() override;
		std::string lifeBarName;
		std::shared_ptr<GameMaster> gameMaster;

	public:
		PlayerLife(Engine::GameObject& gameObject);

		void start() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}