#include "physic_manager.hpp"

#include <algorithm>

#include "debug.hpp"
#include "time.hpp"
#include "graph.hpp"

#include "intersection.h"
#include "utils.hpp"
#include "collision.hpp"

namespace Physics
{
	PhysicManager::PhysicManager()
	{
		Core::Debug::Log::info("Creating the Physic Manager");
	}

	PhysicManager::~PhysicManager()
	{
		Core::Debug::Log::info("Destroying the Physic Manager");
	}

	std::vector<RaycastHit> PhysicManager::raycastAll(const Ray& ray)
	{
		PhysicManager* PM = instance();

		std::vector<RaycastHit> raycastHits;

		vec3 segmentOrigin = ray.origin;
		vec3 segmentEnding = ray.origin + ray.direction.normalized() * ray.distance;

		for (auto& boxCollider : PM->boxColliders)
		{
			if (!boxCollider->isActive() || boxCollider->isTrigger)
				continue;

			boxCollider->updateShape();

			RaycastHit rayHit { boxCollider };

			Box newBox = boxCollider->box;
			newBox.center = boxCollider->m_center;
			newBox.size = boxCollider->extensions;
			newBox.quaternion = Core::Maths::quaternionFromEuler(boxCollider->m_transform->m_rotation);

			if (IntersectSegmentBox(segmentOrigin, segmentEnding, newBox, rayHit.hit))
			{
				rayHit.distance = squaredDistance(ray.origin, rayHit.hit.point);
				raycastHits.push_back(rayHit);
			}
		}

		for (auto& sphereCollider : PM->sphereColliders)
		{
			if (!sphereCollider->isActive())
				continue;

			sphereCollider->updateShape();

			RaycastHit rayHit { sphereCollider };

			Sphere newSphere = sphereCollider->sphere;
			newSphere.center = sphereCollider->m_center;
			newSphere.radius = sphereCollider->extensions.x;
			newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->m_rotation);

			if (IntersectSegmentSphere(segmentOrigin, segmentEnding, newSphere, rayHit.hit))
			{
				rayHit.distance = squaredDistance(ray.origin, rayHit.hit.point);
				raycastHits.push_back(rayHit);
			}
		}

		return raycastHits;
	}

	bool PhysicManager::raycast(const Ray& ray, RaycastHit& raycastHit)
	{
		std::vector<RaycastHit> raycastHits = raycastAll(ray);

		if (raycastHits.empty())
			return false;

		raycastHit = raycastHits[0];

		for (int i = 1; i < raycastHits.size(); i++)
			if (raycastHits[i].distance < raycastHit.distance)
				raycastHit = raycastHits[i];

		return true;
	}

	void PhysicManager::linkComponent(const std::shared_ptr<BoxCollider> compToLink)
	{
		PhysicManager* PM = instance();

		if (compToLink->hasRigidbody())
		{
			PM->boxColliders.insert(PM->boxColliders.begin(), compToLink);
			PM->lastBoxRigidbodyIndex++;
			return;
		}

		PM->boxColliders.insert(PM->boxColliders.end(), compToLink);
	}

	void PhysicManager::removeComponent(BoxCollider* compToRemove)
	{
		PhysicManager* PM = instance();

		for (auto it = PM->boxColliders.begin(); it != PM->boxColliders.end(); it++)
		{
			if (it->get() == compToRemove)
			{

				int a = it->use_count();

				if (it->get()->hasRigidbody())
					PM->lastBoxRigidbodyIndex--;

				PM->boxColliders.erase(it);
				break;
			}
		}
	}

	void PhysicManager::linkComponent(const std::shared_ptr<SphereCollider> compToLink)
	{
		PhysicManager* PM = instance();

		if (compToLink->hasRigidbody())
		{
			PM->sphereColliders.insert(PM->sphereColliders.begin(), compToLink);
			PM->lastSphereRigidbodyIndex++;
			return;
		}

		PM->sphereColliders.insert(PM->sphereColliders.end(), compToLink);
	}

	void PhysicManager::removeComponent(SphereCollider* compToRemove)
	{
		PhysicManager* PM = instance();

		for (auto it = PM->sphereColliders.begin(); it != PM->sphereColliders.end(); it++)
		{
			if (it->get() == compToRemove)
			{
				if (it->get()->hasRigidbody())
					PM->lastSphereRigidbodyIndex--;

				PM->sphereColliders.erase(it);
				break;
			}
		}
	}

	void PhysicManager::computeCollisions()
	{
		// Sort the part of the colliders linked to a rigidbody
		auto spherePartition = std::partition(sphereColliders.begin(), sphereColliders.begin() + lastSphereRigidbodyIndex, [](auto p) { return p->isRigidbodyAwake(); });

		for (auto sphereColliderIt = sphereColliders.begin(); sphereColliderIt != spherePartition; sphereColliderIt++)
		{
			auto sphereCollider = *sphereColliderIt;

			if (!sphereCollider->isActive())
				continue;

			for (auto sphereToCheckIt = sphereColliders.begin(); sphereToCheckIt != sphereColliders.end(); sphereToCheckIt++)
			{
				// Avoid sphere colliding with itself
				if (!sphereCollider->isActive() || sphereToCheckIt == sphereColliderIt)
					continue;

				auto sphereToCheck = *sphereToCheckIt;

				sphereCollider->updateShape();
				sphereToCheck->updateShape();

				Sphere newSphere = sphereCollider->sphere;
				newSphere.center = sphereCollider->m_center;
				newSphere.radius = sphereCollider->extensions.x;
				newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->m_rotation);

				Sphere newSphereToCheck = sphereToCheck->sphere;
				newSphereToCheck.center = sphereToCheck->m_center;
				newSphereToCheck.radius = sphereToCheck->extensions.x;
				newSphereToCheck.quaternion = Core::Maths::quaternionFromEuler(sphereToCheck->m_transform->m_rotation);

				if (sphereCollider->isTrigger || sphereToCheck->isTrigger)
				{
					bool hasHit = TriggerSpheres(newSphere, newSphereToCheck);
					sphereCollider->computeTriggerCallback(hasHit, sphereToCheck.get());
					sphereToCheck->computeTriggerCallback(hasHit, sphereCollider.get());
					continue;
				}

				Hit hit;
				bool hasHit = IntersectSpheres(newSphere, sphereCollider->m_rigidbody->getNewPosition(newSphere.center), newSphereToCheck, hit);

				sphereCollider->computeCollisionCallback(hasHit, { sphereToCheck.get(), hit });
				sphereToCheck->computeCollisionCallback(hasHit, { sphereCollider.get(), hit });
			}

			for (auto& boxCollider : boxColliders)
			{
				if (!boxCollider->isActive())
					continue;

				sphereCollider->updateShape();
				boxCollider->updateShape();

				Box newBox = boxCollider->box;
				newBox.center = boxCollider->m_center;
				newBox.size = boxCollider->extensions;
				newBox.quaternion = Core::Maths::quaternionFromEuler(boxCollider->m_transform->m_rotation);

				Sphere newSphere = sphereCollider->sphere;
				newSphere.center = sphereCollider->m_center;
				newSphere.radius = sphereCollider->extensions.x;
				newSphere.quaternion = Core::Maths::quaternionFromEuler(sphereCollider->m_transform->m_rotation);

				if (sphereCollider->isTrigger || boxCollider->isTrigger)
				{
					bool hasHit = TriggerSphereBox(newSphere, newBox);
					sphereCollider->computeTriggerCallback(hasHit, boxCollider.get());
					boxCollider->computeTriggerCallback(hasHit, sphereCollider.get());
					continue;
				}

				Hit hit;
				bool hasHit = IntersectSphereBox(newSphere, sphereCollider->m_rigidbody->getNewPosition(newSphere.center), newBox, hit);

				sphereCollider->computeCollisionCallback(hasHit, { boxCollider.get(), hit });
				boxCollider->computeCollisionCallback(hasHit, { sphereCollider.get(), hit });
			}

			sphereCollider->m_rigidbody->computeNextPos();
		}
	}

	void PhysicManager::update()
	{
		PhysicManager* PM = instance();

		PM->timeStocker += Core::TimeManager::getDeltaTime();

		float fixedDeltaTime = Core::TimeManager::getFixedDeltaTime();

		// Fixed loop
		while (PM->timeStocker >= fixedDeltaTime)
		{
			PM->timeStocker -= fixedDeltaTime;

			// Call fixed update for all components
			Core::Engine::Graph::fixedUpdate();

			PM->computeCollisions();
		}
	}
}