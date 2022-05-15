#include "shape.hpp"

namespace Physics
{
	Shape::Shape(const Core::Maths::vec3& origin, const quat& _quaternion)
		: center(origin), quaternion(_quaternion)
	{
	}
}