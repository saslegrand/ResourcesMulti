#include "life_bar.hpp"

#include "graph.hpp"
#include "entity_life.hpp"

namespace Gameplay
{
	LifeBar::LifeBar(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<LifeBar>(this))
	{

	}

	void LifeBar::start()
	{
		pivot = getHost().getComponent<Physics::Transform>()->getChild(0);
	}

	void LifeBar::updateSprite(int life, int maxLife)
	{
		if (life < 0)
			return;

		pivot->m_scale.x = (float)life / (float)maxLife;
	}

	std::string LifeBar::toString() const
	{
		return "COMP LIFEBAR ";
	}

	void LifeBar::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<LifeBar> lb;
		if (!gameObject.tryGetComponent(lb))
			lb = gameObject.addComponent<LifeBar>();
	}
}