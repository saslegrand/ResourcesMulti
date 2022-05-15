#include <cfloat>
#include <algorithm>

#include "intersection.h"
#include "referential.h"
#include "utils.hpp"

#define ZERO			0.000001f
#define INTERSECTION	true
#define NO_INTERSECTION false

namespace Physics
{
	void GetUnitAxesFromQuaternion(vec3& i, vec3& j, vec3& k, const quat& quaternion)
	{
		i = vectorRotate({ 1,0,0 }, quaternion).normalized();
		j = vectorRotate({ 0,1,0 }, quaternion).normalized();
		k = vectorRotate({ 0,0,1 }, quaternion).normalized();
	}

	// Check if there is an intersection between a segment and a quad
	bool IntersectSegmentQuad(const vec3& A, const vec3& B, const Quad& quad, Hit& hit)
	{
		// Check if the segment and the normal of the quad are in the same direction
		vec3 j = vectorRotate({ 0,1,0 }, quad.quaternion).normalized();
		if (dot(B - A, j) > 0.f) return NO_INTERSECTION;

		// Check if the segment intersect with the plane (from the quad)
		if (!IntersectSegmentPlane(A, B, Plane(j, quad.center), hit)) return NO_INTERSECTION;

		// Check if the intersection point (from the plane) is on the extensions of the quad
		vec3 ItQuadOrigin = hit.point - quad.center;
		vec3 i = vectorRotate({ 1,0,0 }, quad.quaternion).normalized();
		if (fabsf(dot(ItQuadOrigin, i)) > quad.size.x) return NO_INTERSECTION;
		vec3 k = vectorRotate({ 0,0,1 }, quad.quaternion).normalized();
		if (fabsf(dot(ItQuadOrigin, k)) > quad.size.z) return NO_INTERSECTION;

		return INTERSECTION;
	}

	// Check if there is an intersection between a segment and a vorono region (for roundedBoxes)
	bool IntersectSegmentVoronoiRegion(const vec3& A, const vec3& B, Quad& quad, float roundingOffset, Hit& hit)
	{
		// Reduce extensions of the quad from the OBB
		quad.size = vec3(quad.size.x - roundingOffset, 0.f, quad.size.z - roundingOffset);

		// Check if intersect on voronoi face
		if (IntersectSegmentQuad(A, B, quad, hit)) return INTERSECTION;

		// Calculate quad referential
		vec3 i, j, k;
		GetUnitAxesFromQuaternion(i, j, k, quad.quaternion);

		Capsule capsule;
		vec3 vectInterPtQuadPos = hit.point - quad.center;
		float IdotInterPtQuad = dot(i, vectInterPtQuadPos);
		float KdotInterPtQuad = dot(k, vectInterPtQuadPos);

		bool isOnXAxis = (quad.size.x - fabsf(IdotInterPtQuad)) < (quad.size.z - fabsf(KdotInterPtQuad));
		// Determine which capsule is concern by the potential intersection
		if (isOnXAxis)   // --> Horizontal capsule or vertical capsule (then side determine by the sign of the dotProduct)
		{
			// Left or right capsule
			capsule = Capsule(quad.center - j * roundingOffset + i * quad.size.x * Utils::sign(dot(i, vectInterPtQuadPos)), quad.size.z * 2.f, roundingOffset,
				quad.quaternion * quat({ 1,0,0 }, PI * 0.5f));
		}
		else
		{
			// Up or down capsule
			capsule = Capsule(quad.center - j * roundingOffset + k * quad.size.z * Utils::sign(dot(k, vectInterPtQuadPos)), quad.size.x * 2.f, roundingOffset,
				quad.quaternion * quat({ 0,0,1 }, PI * 0.5f));
		}

		/*Capsule capsuleA = Capsule(quad.center - j * roundingOffset + i * quad.size.x * Utils::sign(dot(i, vectInterPtQuadPos)), quad.size.z * 2.f, roundingOffset,
			quad.quaternion * quat({ 1,0,0 }, PI * 0.5f));
		Capsule capsuleB = Capsule(quad.center - j * roundingOffset + k * quad.size.z * Utils::sign(dot(k, vectInterPtQuadPos)), quad.size.x * 2.f, roundingOffset,
			quad.quaternion * quat({ 0,0,1 }, PI * 0.5f));

		if (IntersectSegmentCapsule(capsuleA, A, B, hit) ||
			IntersectSegmentCapsule(capsuleB, A, B, hit))
			return INTERSECTION;*/

		if (IntersectSegmentCapsule(capsule, A, B, hit))
			return INTERSECTION;

		return NO_INTERSECTION;
	}

	bool IntersectSegmentRoundedBox(const vec3& A, const vec3& B, const Box& box, Hit& hit)
	{
		// Box referential
		vec3 i, j, k;
		GetUnitAxesFromQuaternion(i, j, k, box.quaternion);

		// Segment AB
		vec3 AB = B - A;

		// Is collide with front face
		if (dot(k, AB) <= 0.f)
		{
			// Front quad
			Quad quad = Quad(vectorRotate({ 0,0,box.size.z }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.y }, box.quaternion * quat({ 1,0,0 }, PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
			{
				if (IntersectSegmentVoronoiRegion(A, B, quad, box.offsetRounding, hit))
					return INTERSECTION;
			}
		}
		// Is collide with back face
		else
		{
			Quad quad = Quad(vectorRotate({ 0,0,-box.size.z }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.y }, box.quaternion * quat({ 1,0,0 }, -PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
			{
				if (IntersectSegmentVoronoiRegion(A, B, quad, box.offsetRounding, hit))
					return INTERSECTION;
			}
		}

		// Is collide with right face
		if (dot(i, AB) <= 0.f)
		{
			Quad quad = Quad(vectorRotate({ box.size.x  ,0,0 }, box.quaternion) + box.center,
				{ box.size.y, 0.f, box.size.z }, box.quaternion * quat({ 0,0,1 }, -PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
			{
				if (IntersectSegmentVoronoiRegion(A, B, quad, box.offsetRounding, hit))
					return INTERSECTION;
			}
		}
		// Is collide with left face
		else
		{
			Quad quad = Quad(vectorRotate({ -box.size.x ,0,0 }, box.quaternion) + box.center,
				{ box.size.y, 0.f, box.size.z }, box.quaternion * quat({ 0,0,1 }, PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
			{
				if (IntersectSegmentVoronoiRegion(A, B, quad, box.offsetRounding, hit))
					return INTERSECTION;
			}
		}

		// Is collide with up face
		if (dot(j, AB) <= 0.f)
		{
			Quad quad = Quad(vectorRotate({ 0,box.size.y ,0 }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.z }, box.quaternion);

			if (IntersectSegmentQuad(A, B, quad, hit))
			{
				if (IntersectSegmentVoronoiRegion(A, B, quad, box.offsetRounding, hit))
					return INTERSECTION;
			}
		}
		// Is collide with down face
		else
		{
			Quad quad = Quad(vectorRotate({ 0,-box.size.y ,0 }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.z }, box.quaternion * quat({ 1,0,0 }, PI));

			if (IntersectSegmentQuad(A, B, quad, hit))
			{
				if (IntersectSegmentVoronoiRegion(A, B, quad, box.offsetRounding, hit))
					return INTERSECTION;
			}
		}

		return NO_INTERSECTION;
	}

	// Check if a point is in or out of a box
	bool IntersectPointBox(const vec3& point, const Box& box)
	{
		// Get referential
		vec3 i, j, k;
		GetUnitAxesFromQuaternion(i, j, k, box.quaternion);

		vec3 pointOriginBox = box.center - point;
		if (fabsf(dot(i, pointOriginBox)) > box.size.x) return NO_INTERSECTION;
		if (fabsf(dot(j, pointOriginBox)) > box.size.y) return NO_INTERSECTION;
		if (fabsf(dot(k, pointOriginBox)) > box.size.z) return NO_INTERSECTION;

		// Point inside box
		return INTERSECTION;
	}

	bool IntersectPointRect(const vec2& point, const Rect2D& rect)
	{
		return (fabsf(point.x - rect.center.x) < rect.halfWidth && fabsf(point.y - rect.center.y) < rect.halfHeight);
	}

	bool IntersectSegmentBox(const vec3& A, const vec3& B, const Box& box, Hit& hit)
	{
		// Box referential
		vec3 i, j, k;
		GetUnitAxesFromQuaternion(i, j, k, box.quaternion);

		// Segment AB
		vec3 AB = B - A;

		// Is collide with front face
		if (dot(k, AB) <= 0.f)
		{
			// Front quad
			Quad quad = Quad(vectorRotate({ 0,0,box.size.z }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.y }, box.quaternion * quat({ 1,0,0 }, PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
				return INTERSECTION;
		}
		// Is collide with back face
		else
		{
			Quad quad = Quad(vectorRotate({ 0,0,-box.size.z }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.y }, box.quaternion * quat({ 1,0,0 }, -PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
				return INTERSECTION;
		}

		// Is collide with right face
		if (dot(i, AB) <= 0.f)
		{
			Quad quad = Quad(vectorRotate({ box.size.x  ,0,0 }, box.quaternion) + box.center,
				{ box.size.y, 0.f, box.size.z }, box.quaternion * quat({ 0,0,1 }, -PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
				return INTERSECTION;
		}
		// Is collide with left face
		else
		{
			Quad quad = Quad(vectorRotate({ -box.size.x ,0,0 }, box.quaternion) + box.center,
				{ box.size.y, 0.f, box.size.z }, box.quaternion * quat({ 0,0,1 }, PI * 0.5f));

			if (IntersectSegmentQuad(A, B, quad, hit))
				return INTERSECTION;
		}

		// Is collide with up face
		if (dot(j, AB) <= 0.f)
		{
			Quad quad = Quad(vectorRotate({ 0,box.size.y ,0 }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.z }, box.quaternion);

			if (IntersectSegmentQuad(A, B, quad, hit))
				return INTERSECTION;
		}
		// Is collide with down face
		else
		{
			Quad quad = Quad(vectorRotate({ 0,-box.size.y ,0 }, box.quaternion) + box.center,
				{ box.size.x, 0.f, box.size.z }, box.quaternion * quat({ 1,0,0 }, PI));

			if (IntersectSegmentQuad(A, B, quad, hit))
				return INTERSECTION;
		}

		return NO_INTERSECTION;
	}

	// Return intersection netween a plane and a segment
	bool IntersectSegmentPlane(const vec3& A, const vec3& B, const Plane& plane, Hit& hit)
	{
		// Segment AB
		vec3 AB = B - A;

		// Check if the sement is // with the plane
		float dotABNormal = dot(AB, plane.normal);
		if (fabsf(dotABNormal) < ZERO) return NO_INTERSECTION;

		float t = dot((plane.normal * plane.distance) - A, plane.normal) / dotABNormal;
		if (t < 0 || t > 1) return NO_INTERSECTION;    // Check if intersection is on the segment AB

		// Get intersection infos
		hit.point = A + (AB * t);
		hit.normal = plane.normal;

		return INTERSECTION;
	}

	// Return intersection netween a plane and a segment
	bool IntersectSegmentDisk(const vec3& A, const vec3& B, const Disk& disk, Hit& hit)
	{
		// Check if segment intersect with the plane of the disk
		if (!IntersectSegmentPlane(A, B, Plane(disk.normal, disk.center), hit))
			return NO_INTERSECTION;

		// Check if the intersection is on the disk
		vec3 omegaI = hit.point - disk.center;
		if (dot(omegaI, omegaI) > disk.radius * disk.radius) return NO_INTERSECTION;

		return INTERSECTION;
	}

	// Return intersection netween a plane and a line
	bool IntersectLinePlane(const vec3& A, const vec3& B, const Plane& plane, Hit& hit)
	{
		// Segment AB (give the line direction)
		vec3 AB = B - A;

		// Check if the sement is // with the plane
		float dotABNormal = dot(AB, plane.normal);
		if (fabsf(dotABNormal) < ZERO) return NO_INTERSECTION;

		float t = dot((plane.normal * plane.distance) - A, plane.normal) / dotABNormal;

		// Get intersection infos
		hit.point = A + (AB * t);
		hit.normal = plane.normal;

		return INTERSECTION;
	}

	// Return intersection between a sphere and a segment (in direction of AB)
	bool IntersectSegmentSphere(const vec3& A, const vec3& B, const Sphere& sphere, Hit& hit)
	{
		// Segment AB
		vec3 AB = B - A;
		vec3 omegaA = A - sphere.center;

		// Constant values
		float a = dot(AB, AB);

		if (a == 0.f)
			return NO_INTERSECTION;

		float b = 2.f * dot(omegaA, AB);
		float c = dot(omegaA, omegaA) - (sphere.radius * sphere.radius);

		// Calculate delta
		float delta = (b * b) - (4 * a * c);
		if (delta < 0.f) return NO_INTERSECTION;

		// Calculate the offset of the intersection point from A
		float t = (-b - sqrtf(delta)) / (2.f * a);
		// Check if intersect on the segment
		if (t < 0 || t > 1) return NO_INTERSECTION;

		// Calculate intersection point and intersection normal
		hit.point = A + AB * t;
		hit.normal = (hit.point - sphere.center).normalized();

		return INTERSECTION;
	}

	bool IntersectSpheres(const Sphere& sphereA, const vec3& newSphereAPos, const Sphere& sphereB, Hit& hit)
	{
		Sphere newSphereB = sphereB;
		newSphereB.radius += sphereA.radius;

		// Avoid segment point A inside sphereB
		vec3 offsetSphereOrigin = (newSphereAPos - sphereA.center).normalized() * sphereA.radius;

		return IntersectSegmentSphere(sphereA.center - offsetSphereOrigin, newSphereAPos, newSphereB, hit);
	}

	bool TriggerSpheres(const Sphere& sphereA, const Sphere& sphereB)
	{
		float sumRadius = sphereA.radius + sphereB.radius;
		return (sphereA.center - sphereB.center).squaredMagnitude() <= (sumRadius * sumRadius);
	}

	bool TriggerSphereBox(const Sphere& sphere, const Box& box)
	{
		Referential3D boxRef = Referential3D(box.center, box.quaternion);

		vec3 sphereOriginInBoxRef = boxRef.globalToLocalRef(sphere.center);
		vec3 distances = box.size + sphere.radius;

		// Check if box AABB and sphere AABB triggered
		if (sphereOriginInBoxRef.x > distances.x || sphereOriginInBoxRef.y > distances.y || sphereOriginInBoxRef.z > distances.z)
			return NO_INTERSECTION;

		vec3 closestPointFromSphere;
		closestPointFromSphere.x = std::clamp(sphereOriginInBoxRef.x, -box.size.x, box.size.x);
		closestPointFromSphere.y = std::clamp(sphereOriginInBoxRef.y, -box.size.y, box.size.y);
		closestPointFromSphere.z = std::clamp(sphereOriginInBoxRef.z, -box.size.z, box.size.z);

		return (sphereOriginInBoxRef - closestPointFromSphere).squaredMagnitude() < (sphere.radius * sphere.radius);
	}

	// Return intersection between a segment and a cylinder infinite
	bool IntersectSegmentCylinderInf(const Cylinder& cylinder, const vec3& ptA, const vec3& ptB, Hit& hit)
	{
		// Segment AB
		vec3 AB = ptB - ptA;
		vec3 PQ = vectorRotate(cylinder.ptB, cylinder.quaternion) - vectorRotate(cylinder.ptA, cylinder.quaternion);
		vec3 PA = ptA - cylinder.ptA;

		// Dot products
		float PQdotAB = dot(PQ, AB);
		float PAdotPQ = dot(PA, PQ);
		float PQdotPQ = dot(PQ, PQ);

		// Check if the segment and cylinder are colinear
		float a = dot(AB, AB) * PQdotPQ - (PQdotAB * PQdotAB);
		if (fabsf(a) <= ZERO) return NO_INTERSECTION;

		// Check if the point A is inside the cylinder
		float c = (PQdotPQ * (dot(PA, PA) - (cylinder.radius * cylinder.radius))) - (PAdotPQ * PAdotPQ);
		if (c < 0.f) return NO_INTERSECTION;

		float b = dot(PA, AB) * PQdotPQ - (PQdotAB * PAdotPQ);

		// Calculate delta from 2nd degrees equation - at`2 + 2bt + c = 0 --> delta = b`2 - a * c
		float delta = (b * b) - a * c;

		if (delta < 0.f) return NO_INTERSECTION;

		// Check if the intersection is on the segment
		float t = (-b - sqrtf(delta)) / a;
		if (t < 0 || t > 1) return NO_INTERSECTION;

		// Get intersection point
		hit.point = ptA + (AB * t);

		hit.normal = getVectorPerpendicular(hit.point - cylinder.ptA, PQ).normalized();

		return INTERSECTION;
	}

	// Return intersection between a segment and a cylinder finish
	bool IntersectSegmentCylinder(const Cylinder& cylinder, const vec3& ptA, const vec3& ptB, Hit& hit)
	{
		// Segment AB
		vec3 AB = ptB - ptA;
		// Cylinder axe
		vec3 PQ = cylinder.ptB - cylinder.ptA;
		vec3 PA = ptA - cylinder.ptA;

		float PQdotAB = dot(PQ, AB);
		float PAdotPQ = dot(PA, PQ);
		float PQdotPQ = dot(PQ, PQ);

		float a = dot(AB, AB) * PQdotPQ - (PQdotAB * PQdotAB);
		float c = (PQdotPQ * (dot(PA, PA) - cylinder.radius * cylinder.radius)) - (PAdotPQ * PAdotPQ);
		if (fabsf(a) <= ZERO && c > 0.f) return NO_INTERSECTION;		// Segment/Cylinder colinear and A in cylinder

		// A not in the cylinder, test "beyonds"
		if (c < 0.f)
		{
			// Look if point A is on the left of the cylinder
			if (PAdotPQ < 0.f)
				return IntersectSegmentDisk(ptA, ptB, Disk(-PQ.normalized(), cylinder.ptA, cylinder.radius), hit);
			// Look if point A is on the right of the cylinder
			if (PAdotPQ > PQdotPQ)
				return IntersectSegmentDisk(ptA, ptB, Disk(PQ.normalized(), cylinder.ptB, cylinder.radius), hit);

			return NO_INTERSECTION;
		}

		float b = dot(PA, AB) * PQdotPQ - (PQdotAB * PAdotPQ);

		// Calculate delta from 2nd degrees equation - at`2 + 2bt + c = 0 --> delta = b`2 - a * c
		float delta = (b * b) - a * c;
		if (delta < 0.f) return NO_INTERSECTION;

		// Check if the intersection is on the segment
		float t0 = (-b - sqrtf(delta)) / a;
		if (t0 < 0.f || t0 > 1.f) return NO_INTERSECTION;

		// Get Intersectio n point
		hit.point = ptA + AB * t0;

		float PIdotPQ = dot(hit.point - cylinder.ptA, PQ);
		// Check if the intersectionPoint is ont the left or the right disk of the cylinder
		if (PIdotPQ < 0.f)
			return IntersectSegmentDisk(ptA, ptB, Disk(-PQ.normalized(), cylinder.ptA, cylinder.radius), hit);
		if (PIdotPQ > PQdotPQ)
			return IntersectSegmentDisk(ptA, ptB, Disk(PQ.normalized(), cylinder.ptB, cylinder.radius), hit);

		hit.normal = getVectorPerpendicular(hit.point - cylinder.ptA, PQ).normalized();

		return INTERSECTION;
	}

	// Return intersection between a segment and a capsule
	bool IntersectSegmentCapsule(const Capsule& caps, const vec3& ptA, const vec3& ptB, Hit& hit)
	{
		// Segment AB
		vec3 AB = ptB - ptA;
		// Capsule axe
		vec3 PQ = caps.ptB - caps.ptA;
		vec3 PA = ptA - caps.ptA;

		float PQdotAB = dot(PQ, AB);
		float PAdotPQ = dot(PA, PQ);
		float PQdotPQ = dot(PQ, PQ);

		float a = dot(AB, AB) * PQdotPQ - (PQdotAB * PQdotAB);
		float c = (PQdotPQ * (dot(PA, PA) - caps.radius * caps.radius)) - (PAdotPQ * PAdotPQ);
		if (fabsf(a) <= ZERO && c > 0.f) return NO_INTERSECTION;		// Segment/Cylinder colinear and A in cylinder

		// A not in the cylinder, test "beyonds" capsule
		if (c < 0.f)
		{
			// Look if point A is on the left of the capsule
			if (PAdotPQ < 0.f)
				return IntersectSegmentSphere(ptA, ptB, Sphere(caps.ptA, caps.radius, caps.quaternion), hit);
			// Look if point A is on the right of the capsule
			if (PAdotPQ > PQdotPQ)
				return IntersectSegmentSphere(ptA, ptB, Sphere(caps.ptB, caps.radius, caps.quaternion), hit);

			return NO_INTERSECTION;
		}

		float b = dot(PA, AB) * PQdotPQ - (PQdotAB * PAdotPQ);

		// at`2 + 2bt + c = 0 --> delta = b`2 - a * c
		float delta = (b * b) - a * c;

		if (delta < 0.f) return NO_INTERSECTION;

		// Check if the intersection is on the segment
		float t0 = (-b - sqrtf(delta)) / a;
		if (t0 < 0.f || t0 > 1.f) return NO_INTERSECTION;

		// Get the intersection point from the segment
		hit.point = ptA + AB * t0;

		float PIdotPQ = dot(hit.point - caps.ptA, PQ);
		// Check if the intersectionPoint is ont the left or the right disk of the cylinder
		if (PIdotPQ < 0.f)
			return IntersectSegmentSphere(ptA, ptB, Sphere(caps.ptA, caps.radius, caps.quaternion), hit);
		if (PIdotPQ > PQdotPQ)
			return IntersectSegmentSphere(ptA, ptB, Sphere(caps.ptB, caps.radius, caps.quaternion), hit);
		

		hit.normal = getVectorPerpendicular(hit.point - caps.ptA, PQ).normalized();

		return INTERSECTION;
	}

	// Check if there is an intersection between a sphere and a box
	bool IntersectSphereBox(const Sphere& sphere, const vec3& newSpherePos, const Box& box, Hit& hit)
	{
		Box roundedBox = box;
		roundedBox.offsetRounding = sphere.radius;			    // Set rounding as the radius to consider the sphere
		roundedBox.size += vec3(sphere.radius, sphere.radius, sphere.radius);  // Set new size

		// Avoid segment point A inside roundedBox OBB
		vec3 offsetSphereOrigin = (newSpherePos - sphere.center).normalized() * (sphere.radius + 0.1f);

		return IntersectSegmentRoundedBox(sphere.center - offsetSphereOrigin, newSpherePos, roundedBox, hit);
	}

	bool RaycastBox(const Ray& ray, const Box& box, Hit& hit)
	{
		return IntersectSegmentBox(ray.origin, ray.origin + ray.direction.normalized() * ray.distance, box, hit);
	}

	bool RaycastSphere(const Ray& ray, const Sphere& sphere, Hit& hit)
	{
		return IntersectSegmentSphere(ray.origin, ray.origin + ray.direction.normalized() * ray.distance, sphere, hit);
	}
}