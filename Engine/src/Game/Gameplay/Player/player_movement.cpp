#include "player_movement.hpp"

#include "benchmark.hpp"

namespace Gameplay
{
	PlayerMovement::PlayerMovement(Engine::GameObject& gameObject)
		: EntityMovement(gameObject, std::shared_ptr<PlayerMovement>(this))
	{
		m_transform = requireComponent<Physics::Transform>();
	}

	void PlayerMovement::start()
	{
		m_cameraTransform = Core::Engine::Graph::findGameObjectWithName("MainCamera")->getComponent<Physics::Transform>();
		bench = Core::Engine::Benchmark::getInstance();
	}

	void PlayerMovement::fixedUpdate()
	{
		if (bench->isActive())
			return;

		float fixedSpeed = m_speed * Core::TimeManager::getFixedDeltaTime();
		float horizontal = Core::Input::InputManager::getAxis("Horizontal");
		float vertical = Core::Input::InputManager::getAxis("Forward");

		fixedSpeed *= Core::Input::InputManager::getButton("Run") ? 2.f : 1.f;

		m_transform->m_rotation.y -= m_sensivityY * Core::TimeManager::getFixedDeltaTime() * Core::Input::InputManager::getDeltasMouse().x;

		float cos = cosf(m_transform->m_rotation.y), sin = sinf(m_transform->m_rotation.y);
		Core::Maths::vec3 newVelocity = Core::Maths::vec3(horizontal * cos + vertical * sin, 0.f, vertical * cos - horizontal * sin).normalized() * fixedSpeed;
		//newVelocity.y = m_rigidbody->velocity.y;
		//m_rigidbody->velocity = newVelocity;

		m_transform->m_position.x += newVelocity.x;
		m_transform->m_position.z += newVelocity.z;

		if (Core::Input::InputManager::getButton("Jump"))
			m_transform->m_position.y += fixedSpeed;
		if (Core::Input::InputManager::getButton("Sneak"))
			m_transform->m_position.y -= fixedSpeed;
	}

	void PlayerMovement::drawImGui()
	{
		if (ImGui::TreeNode("PlayerMovement"))
		{
			ImGui::DragFloat("Sensivity Y : ", &m_sensivityY);
			ImGui::DragFloat("MoveSpeed : ", &m_speed);
			ImGui::TreePop();
		}
	}

	std::string PlayerMovement::toString() const
	{
		return "COMP PLAYERMOVEMENT " + std::to_string(m_speed) + " " + " " + std::to_string(m_sensivityY);
	}

	void PlayerMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<PlayerMovement> player;
		if (!gameObject.tryGetComponent(player))
			player = gameObject.addComponent<PlayerMovement>();

		iss >> player->m_speed;
		iss >> player->m_sensivityY;
	}
}