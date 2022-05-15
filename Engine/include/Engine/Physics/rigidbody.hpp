#pragma once

#include <memory>

#include "component.hpp"
#include "maths.hpp"

#include "transform.hpp"

namespace Physics
{
	class Rigidbody : public Engine::Component
	{
	private:
		Rigidbody(Engine::GameObject& gameObject, std::shared_ptr<Rigidbody> ptr);

		Core::Maths::vec3 acceleration = Core::Maths::vec3(0.f, 0.f, 0.f);

		std::shared_ptr<Transform> m_transform = nullptr;

		Core::Maths::vec3 forceSum;

	public:
		Core::Maths::vec3 velocity = Core::Maths::vec3(0.f, 0.f, 0.f);
		Core::Maths::vec3 gravity = Core::Maths::vec3(0.f, -9.81f, 0.f);
		float mass = 1.f;

		float drag = 0.5f;

		bool isAwake = false;

		Rigidbody(Engine::GameObject& gameObject);

		void addForce(const Core::Maths::vec3& force);
		Core::Maths::vec3 getNewPosition(const Core::Maths::vec3& center) const;

		void fixedUpdate() override;
		void computeNextPos();
		void drawImGui() override;

		void onCollisionEnter(const Collision& collision) override;
		void onCollisionStay(const Collision& collision) override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}