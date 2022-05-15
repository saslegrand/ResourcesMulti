#include "enemy_life.hpp"

namespace Gameplay
{
	EnemyLife::EnemyLife(Engine::GameObject& gameObject)
		: EntityLife(gameObject, std::shared_ptr<EnemyLife>(this), "resources/sounds/enemyDamage.ogg", "resources/sounds/enemyDeath.wav")
	{
	}

	void EnemyLife::start()
	{
		gameMaster = Core::Engine::Graph::findGameObjectWithName("GameMaster")->getComponent<GameMaster>();
		gameMaster->enemyCount++;
	}

	void EnemyLife::drawImGui()
	{
		if (ImGui::TreeNode("EnemyLife"))
		{
			ImGui::DragInt("Life : ", &life);
			ImGui::TreePop();
		}
	}

	std::string EnemyLife::toString() const
	{
		return "COMP ENEMYLIFE " + EntityLife::toString();
	}

	void EnemyLife::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<EnemyLife> el;
		if (!gameObject.tryGetComponent(el))
			el = gameObject.addComponent<EnemyLife>();

		iss >> el->life;
		iss >> el->maxLife;
	}

	void EnemyLife::hurt(int damage)
	{
		EntityLife::hurt(damage);
	}

	void EnemyLife::kill()
	{
		gameMaster->removeEnemy();

		EntityLife::kill();

		getHost().destroy();
	}
}