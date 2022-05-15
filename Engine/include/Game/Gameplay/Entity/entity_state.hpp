#pragma once

#include <memory>

#include "component.hpp"

namespace Physics
{
	class Transform;
}

namespace Gameplay
{
	class EntityState : public Engine::Component
	{
	protected:
		EntityState(Engine::GameObject& gameObject, std::shared_ptr<EntityState> ptr);

	public:
		std::shared_ptr<Physics::Transform> transform = nullptr;

		bool isIdle = false;
		bool isWalking = false;
		bool isRunning = false;
		bool isJumping = false;
		bool isFalling = false;
		bool isGrounded = false;

		float horizontalMove = 0.f;
		float forwardMove = 0.f;

		int colliderCount = 0;
	};
}