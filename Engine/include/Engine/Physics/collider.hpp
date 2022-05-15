#pragma once

#include <unordered_map>

#include "component.hpp"
#include "rigidbody.hpp"
#include "collision.hpp"
#include "maths.hpp"

namespace Physics
{
	class Collider : public Engine::Component
	{
	protected:
		std::unordered_map<Collider*, Collision> m_colliders;
		std::vector<Collider*> m_triggers;
		Core::Maths::vec3 m_positionOffset;

		Collider(Engine::GameObject& gameObject, std::shared_ptr<Collider> ptr);

	public:
		Core::Maths::vec3 m_center;
		Core::Maths::vec3 extensions;

		std::shared_ptr<Physics::Transform> m_transform = nullptr;
		std::shared_ptr<Physics::Rigidbody> m_rigidbody = nullptr;

		bool isTrigger = false;
		bool isDraw = false;

		bool hasRigidbody();
		bool isRigidbodyAwake();

		void computeCollisionCallback(bool hasHit, const Collision& collision);
		void computeTriggerCallback(bool hasHit, Collider* collider);

		void drawImGui() override;

		virtual void updateShape() = 0;
	};
}