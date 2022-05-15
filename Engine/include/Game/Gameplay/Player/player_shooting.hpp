#pragma once

#include <memory>
#include <algorithm> 
#include <imgui.h>

#include "component.hpp"
#include "rigidbody.hpp"
#include "button.hpp"
#include "maths.hpp"
#include "timer.hpp"

#include "game_master.hpp"

namespace Gameplay
{
	class PlayerShooting : public Engine::Component
	{
	private:
		std::shared_ptr<GameMaster> gameMaster;

		std::shared_ptr<Physics::Transform> m_cameraTransform;
		std::shared_ptr<Physics::Transform> m_weaponTransform;
		Core::Maths::vec3 initRotation;
		Core::Maths::vec3 initPosition;

		int ammo = 7;
		int maxAmmo = 7;

		float speedLerpReload = 3.5f;
		float speedLerpShoot = 10.f;

		float maxShootDistance = 250.f;
		float recoil = 0.2f;

		float fireRate = 0.2f;

		bool isReloading = false;
		bool reload = false;

		void shoot();

	public:
		PlayerShooting(Engine::GameObject& gameObject);

		void shooting();
		void reloading();

		int getMaxAmmoCount();
		void start() override;
		void update() override;
		void drawImGui() override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		Core::Timer timer;
	};
}

