#include "entity_state.hpp"

#include <imgui.h>

#include "rigidbody.hpp"
#include "maths.hpp"
#include "transform.hpp"

#include "inputs_manager.hpp"

namespace Gameplay
{
	EntityState::EntityState(Engine::GameObject& gameObject, std::shared_ptr<EntityState> ptr)
		: Component(gameObject, ptr)
	{
		transform = requireComponent<Physics::Transform>();
	}
}