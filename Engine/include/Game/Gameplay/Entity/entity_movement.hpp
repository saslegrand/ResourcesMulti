#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "maths.hpp"

#include "inputs_manager.hpp"
#include "button.hpp"
#include "graph.hpp"
#include "time.hpp"

namespace Gameplay
{
	class EntityMovement : public Engine::Component
	{
	protected:
		float m_speed = 0.f;

		std::shared_ptr<Physics::Rigidbody> m_rigidbody;
		std::shared_ptr<Physics::Transform> m_transform;

	public:
		EntityMovement(Engine::GameObject& gameObject, std::shared_ptr<EntityMovement> ptr);
	};
}