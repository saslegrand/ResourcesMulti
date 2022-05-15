#pragma once

#include "component.hpp"

#include "timer.hpp"

namespace Gameplay
{
	class BulletHole : public Engine::Component
	{
		Core::Timer lifeTime;

	public:
		BulletHole(Engine::GameObject& gameObject);

		void start() override;
		void update() override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}