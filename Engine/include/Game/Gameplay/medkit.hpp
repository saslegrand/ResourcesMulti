#pragma once

#include "component.hpp"
#include "transform.hpp"

namespace Gameplay
{
	class MedKit : public Engine::Component
	{
	private:

		int healCount = 2;
		std::shared_ptr<Physics::Transform> transform;

	public:
		MedKit(Engine::GameObject& gameObject);

		void start() override;
		void update() override;
		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		void onTriggerEnter(Physics::Collider* collider) override;
	};
}