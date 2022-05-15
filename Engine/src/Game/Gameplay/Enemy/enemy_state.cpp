#include "enemy_state.hpp"

#include <imgui.h>
#include <algorithm>

#include "inputs_manager.hpp"
#include "graph.hpp"

#include "collider.hpp"
#include "rigidbody.hpp"
#include "player_life.hpp"
#include "maths.hpp"


namespace Gameplay
{
	EnemyState::EnemyState(Engine::GameObject& gameObject)
		: EntityState(gameObject, std::shared_ptr<EnemyState>(this)) 
	{
		rb = getHost().getComponent<Physics::Rigidbody>();
	}

	void EnemyState::drawImGui()
	{
		if (ImGui::TreeNode("EnemyState"))
		{
			Component::drawImGui();
			ImGui::TreePop();
		}
	}

	void EnemyState::start()
	{
		playerLife = Core::Engine::Graph::findGameObjectWithName("Player")->getComponent<PlayerLife>();
	}

	void EnemyState::update()
	{
		attackCooldown.update();
	}

	void EnemyState::hurtPlayer()
	{
		rb->velocity.x = rb->velocity.z = 0.f;

		if (attackCooldown.timerOn())
		{
			attackCooldown.setDelay(1.f);
			playerLife->hurt(1);
		}
	}


	void EnemyState::onCollisionEnter(const Physics::Collision& collision)
	{
		if (collision.collider->getHost().m_name == "Player")
			hurtPlayer();
	}

	void EnemyState::onCollisionStay(const Physics::Collision& collision)
	{
		if (collision.collider->getHost().m_name == "Player")
			hurtPlayer();
	}

	std::string EnemyState::toString() const
	{
		return "COMP ENEMYSTATE " + std::to_string(isIdle) 
			+ " " + std::to_string(isWalking)
			+ " " + std::to_string(isFalling)
			+ " " + std::to_string(isGrounded);
	}

	void EnemyState::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<EnemyState> es;
		if (!gameObject.tryGetComponent(es))
			es = gameObject.addComponent<EnemyState>();

		iss >> es->isIdle;
		iss >> es->isWalking;
		iss >> es->isFalling;
		iss >> es->isGrounded;
	}
}