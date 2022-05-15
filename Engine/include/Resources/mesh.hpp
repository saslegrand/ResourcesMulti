#pragma once

#include <vector>
#include <string>
#include <atomic>

#include <glad\glad.h>

#include "resource.hpp"

#include "maths.hpp"

namespace Resources
{
	class Mesh : public Resource
	{
	private:
		GLuint VAO = 0;
		GLuint VBO = 0;

	public:
		~Mesh();

		long verticesCount = 0;

		std::vector<float> attributs;
		std::string name;
		bool isDrawable = false;

		void draw() const;
		void generateVAO();
		void compute(std::vector<Core::Maths::vec3>& vertices, std::vector<Core::Maths::vec3>& texCoords, std::vector<Core::Maths::vec3>& normals, std::vector<unsigned int>& indices);
	};
}
