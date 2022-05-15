#pragma once

#include "box.h"
#include "disk.h"
#include "quad.h"
#include "plane.h"
#include "sphere.h"
#include "capsule.h"
#include "cylinder.h"
#include "rect2D.hpp"
#include "collision.hpp"

namespace Physics
{
	// Intersection from a segment
	bool IntersectSegmentQuad(const vec3& A, const vec3& B, const Quad& quad, Hit& hit);
	bool IntersectSegmentSphere(const vec3& A, const vec3& B, const Sphere& sphere, Hit& hit);
	bool IntersectSegmentBox(const vec3& A, const vec3& B, const Box& box, Hit& hit);
	bool IntersectSegmentRoundedBox(const vec3& A, const vec3& B, const Box& box, Hit& hit);
	bool IntersectSegmentPlane(const vec3& A, const vec3& B, const Plane& plane, Hit& hit);
	bool IntersectLinePlane(const vec3& A, const vec3& B, const Plane& plane, Hit& hit);
	bool IntersectSegmentDisk(const vec3& A, const vec3& B, const Disk& disk, Hit& hit);
	bool IntersectSegmentCylinderInf(const Cylinder& cylinder, const vec3& ptA, const vec3& ptB, Hit& hit);
	bool IntersectSegmentCylinder(const Cylinder& cylinder, const vec3& ptA, const vec3& ptB, Hit& hit);
	bool IntersectSegmentCapsule(const Capsule& caps, const vec3& ptA, const vec3& ptB, Hit& hit);

	// Intersections from a sphere
	bool IntersectSphereBox(const Sphere& sphere, const vec3& newSpherePos, const Box& box, Hit& hit);

	bool IntersectSpheres(const Sphere& sphereA, const vec3& newSphereAPos, const Sphere& sphereB, Hit& hit);

	// Intersections from a sphere
	bool RaycastBox(const Ray& ray, const Box& box, Hit& hit);
	bool RaycastSphere(const Ray& ray, const Sphere& sphere, Hit& hit);

	// Intersections from a point
	bool IntersectPointBox(const vec3& point, const Box& box);

	bool IntersectPointRect(const vec2& point, const Rect2D& rect);

	bool TriggerSpheres(const Sphere& sphereA, const Sphere& sphereB);

	bool TriggerSphereBox(const Sphere& sphere, const Box& box);
}