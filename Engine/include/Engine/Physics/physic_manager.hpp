#pragma once

#include <vector>

#include "singleton.hpp"

#include "rigidbody.hpp"
#include "box_collider.hpp"
#include "sphere_collider.hpp"

namespace Physics
{
	class PhysicManager final : public Singleton<PhysicManager>
	{
		friend Singleton<PhysicManager>;

	private:
		PhysicManager();
		~PhysicManager();

		std::vector<std::shared_ptr<BoxCollider>> boxColliders;
		std::vector<std::shared_ptr<SphereCollider>> sphereColliders;

		void computeCollisions();
		float timeStocker = 0.f;
		int lastBoxRigidbodyIndex = 0;
		int lastSphereRigidbodyIndex = 0;

	public:

		static bool raycast(const Ray& ray, RaycastHit& raycastHit);
		static std::vector<RaycastHit> raycastAll(const Ray& ray);

		static void linkComponent(const std::shared_ptr<SphereCollider> compToLink);
		static void removeComponent(SphereCollider* compToRemove);

		static void linkComponent(const std::shared_ptr<BoxCollider> compToLink);
		static void removeComponent(BoxCollider* compToRemove);


		template <class C>
		static void clearComponents();

		/*template<>
		static void clearComponents<Rigidbody>()
		{
			instance()->rigidbodies.clear();
		}*/

		template<>
		static void clearComponents<SphereCollider>()
		{
			instance()->sphereColliders.clear();
			instance()->lastSphereRigidbodyIndex = 0;
		}

		template<>
		static void clearComponents<BoxCollider>()
		{
			instance()->boxColliders.clear();
			instance()->lastBoxRigidbodyIndex = 0;
		}

		static void update();
	};
}