#include "entity_life.hpp"

#include "sound_manager.hpp"

namespace Gameplay
{
	EntityLife::EntityLife(Engine::GameObject& gameObject, std::shared_ptr<EntityLife> ptr, const std::string& _hurtSound, const std::string& _deathSound)
		: Component(gameObject, ptr), hurtSound(_hurtSound), deathSound(_deathSound)
	{

	}

	void EntityLife::start()
	{
	}

	int EntityLife::getCurrentLife() const
	{
		return life;
	}

	int EntityLife::getMaxLife() const
	{
		return maxLife;
	}

	void EntityLife::hurt(int damage)
	{
		if (!setLife(life - damage))
			Core::Engine::SoundManager::play2D(hurtSound);
	}

	void EntityLife::heal(int heal)
	{
		setLife(life + heal);
		//Core::Engine::SoundManager::getSoundEngine()->play2D(hurtSound.c_str());
	}

	bool EntityLife::setLife(int _life)
	{
		life = std::clamp(_life, 0, maxLife);

		if (lifeBar)
			lifeBar->updateSprite(life, maxLife);

		bool isDead = life == 0;
		if (isDead)
			kill();

		return isDead;
	}

	void EntityLife::drawImGui()
	{
		Component::drawImGui();

		ImGui::DragInt("Current life", &life);
		ImGui::DragInt("Max life", &maxLife);
	}

	void EntityLife::kill()
	{
		Core::Engine::SoundManager::play2D(deathSound.c_str());
	}

	std::string EntityLife::toString() const
	{
		return std::to_string(life) + " " + std::to_string(maxLife);
	}
}