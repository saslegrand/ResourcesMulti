#pragma once
#include <string>

namespace Resources
{
	class Resource
	{
	protected:
		std::string m_filePath;

		Resource() = default;
		Resource(const std::string& filePath);

	public:
		std::string m_name;
		std::string getPath() const;
	};
}