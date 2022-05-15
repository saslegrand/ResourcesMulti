#include "object.hpp"

namespace Engine
{
	bool Object::isActive()
	{
		return isEnable;
	}

	void Object::setActive(bool value)
	{
		isEnable = value;
	}
}