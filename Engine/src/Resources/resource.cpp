#include "resource.hpp"

#include "utils.hpp"

namespace Resources
{
	Resource::Resource(const std::string& filePath)
		: m_filePath(filePath)
	{
	}

	std::string Resource::getPath() const
	{
		return m_filePath;
	}
}