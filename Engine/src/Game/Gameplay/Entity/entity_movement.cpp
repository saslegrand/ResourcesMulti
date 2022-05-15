#include "entity_movement.hpp"

namespace Gameplay
{
	EntityMovement::EntityMovement(Engine::GameObject& gameObject, std::shared_ptr<EntityMovement> ptr)
		: Component(gameObject, ptr)
	{
		m_rigidbody = requireComponent<Physics::Rigidbody>();
	}
}