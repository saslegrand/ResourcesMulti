#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>
#include <string>

#include "component.hpp"
#include "rigidbody.hpp"
#include "life_bar.hpp"
#include "maths.hpp"

#include "inputs_manager.hpp"
#include "button.hpp"
#include "graph.hpp"

namespace Gameplay
{
	class EntityLife : public Engine::Component
	{
	protected:
		EntityLife(Engine::GameObject& gameObject, std::shared_ptr<EntityLife> ptr, const std::string& _hurtSound = "", const std::string& _deathSound = "");
		
		virtual void kill();

		std::shared_ptr<LifeBar> lifeBar;

		std::string hurtSound = "";
		std::string deathSound = "";

		int life = 10;
		int maxLife = 10;

	public:
		virtual bool setLife(int _life);

		virtual void hurt(int damage = 1);
		virtual void heal(int heal = 1);

		void start() override;
		void drawImGui() override;

		std::string toString() const override;

		int getCurrentLife() const;
		int getMaxLife() const;
	};
}