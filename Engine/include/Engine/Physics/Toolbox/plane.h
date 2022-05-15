#pragma once

#include "maths.hpp"

namespace Physics
{
	using namespace Core::Maths;

	class Plane
	{
	public:
		Plane() = default;

		Plane(const vec3& _normal, float _distance)
			: normal(_normal), distance(_distance) {}

		Plane(const vec3& _normal, const vec3& pt)
			: normal(_normal)
		{
			distance = dot(pt, normal);
		}

		Plane(const vec3& ptA, const vec3& ptB, const vec3& ptC)
		{
			normal	 = ((ptB - ptA) ^ (ptC - ptA)).normalized();
			distance = dot(ptA, normal);
		}

		vec3  normal;
		float distance;
		quat quaternion = quat::identity();
	};
}

class Ref3D
{
public:
	Ref3D() : i(1.f, 0.f, 0.f), j(0.f, 1.f, 0.f), k(0.f, 0.f, 1.f) { }
	Ref3D(const Core::Maths::vec3& _i, const Core::Maths::vec3& _j, const Core::Maths::vec3& _k)
		: i(_i), j(_j), k(_k) { }

private:
	Core::Maths::vec3 i, j, k;
};