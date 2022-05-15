#pragma once

#include <string>
#include <vector>
#include <array>
#include <memory>

#include <glad/glad.h>

#include "texture.hpp"

namespace Resources
{
	class CubeMap
	{
	private:
		GLuint ID = -1;

		std::array<unsigned char*, 6> datas;
		std::array<int, 12> dimensions;

	public:
		//CubeMap(const std::vector<std::string>& paths);
		CubeMap() = default;
		~CubeMap();

		bool isDrawable = false;

		void create(const std::vector<std::string>& paths, bool isMulti);
		void generate();

		GLuint getID() const;

		void bind() const;
	};
}