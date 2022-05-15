#pragma once

#include "player_state.hpp" 
#include "entity_movement.hpp"

namespace Gameplay
{
	class PlayerMovement : public EntityMovement
	{
	private:
		float m_sensivityY = 0.1f;
		Core::Engine::Benchmark* bench = nullptr;

		std::shared_ptr<Physics::Transform> m_cameraTransform;

	public:
		PlayerMovement(Engine::GameObject& gameObject);

		void start() override;
		void fixedUpdate() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

	};
}