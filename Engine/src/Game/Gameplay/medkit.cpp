#include "medkit.hpp"

#include <imgui.h>

#include "collider.hpp"
#include "player_life.hpp"

namespace Gameplay
{
	MedKit::MedKit(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<MedKit>(this))
	{

	}

	void MedKit::start()
	{
		transform = getHost().getComponent<Physics::Transform>();
	}

	void MedKit::update()
	{
		
	}

	void MedKit::drawImGui()
	{
		if (ImGui::TreeNode("MedKit"))
		{
			Component::drawImGui();
			ImGui::DragInt("Heal count", &healCount);

			ImGui::TreePop();
		}
	}

	std::string MedKit::toString() const
	{
		return "COMP MEDKIT " + std::to_string(healCount);
	}

	void MedKit::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<MedKit> mk;
		if (!gameObject.tryGetComponent(mk))
			mk = gameObject.addComponent<MedKit>();

		iss >> mk->healCount;
	}

	void MedKit::onTriggerEnter(Physics::Collider* collider)
	{
		if (collider->getHost().m_name == "Player")
		{
			auto playerLife = collider->getHost().getComponent<PlayerLife>();
			if (playerLife->getCurrentLife() < playerLife->getMaxLife())
			{
				playerLife->heal(healCount);
				getHost().destroy();
			}

		}
	}
}