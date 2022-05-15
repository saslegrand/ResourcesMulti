#include "bullet_hole.hpp"

namespace Gameplay
{
	BulletHole::BulletHole(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<BulletHole>(this))
	{

	}

	void BulletHole::start()
	{
		lifeTime.setDelay(1.f);
	}

	void BulletHole::update()
	{
		lifeTime.update();

		if (lifeTime.timerOn())
			getHost().destroy();
	}

	void BulletHole::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		std::shared_ptr<BulletHole> bulletHole;
		if (!gameObject.tryGetComponent(bulletHole))
			gameObject.addComponent<BulletHole>();
	}
}