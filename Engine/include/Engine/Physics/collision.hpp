#pragma once

#include <memory>

#include "maths.hpp"

namespace Physics
{
	class Collider;

	struct Ray
	{
		Ray(const Core::Maths::vec3& origin = Core::Maths::vec3(), const Core::Maths::vec3& direction = Core::Maths::vec3(0.f, 1.f, 0.f), float maxDistance = 1.f)
			: direction(direction), origin(origin), distance(maxDistance) {}

		Core::Maths::vec3 direction;
		Core::Maths::vec3 origin;
		float distance = 0.f;
	};

	struct Hit
	{
		Core::Maths::vec3 point = Core::Maths::vec3();
		Core::Maths::vec3 normal = Core::Maths::vec3();
	};

	struct RaycastHit
	{
		std::shared_ptr<Collider> collider;
		Hit hit;
		float distance = 0.f;
	};

	struct Collision
	{
		Collider* collider;
		Hit hit;
	};
}