#include "mesh.hpp"

#include "graph.hpp"

namespace Resources
{
	void Mesh::draw() const
	{
		// Bind the mesh's VAO and draw it
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(attributs.size()));
		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		if (VAO)
			glDeleteVertexArrays(1, &VAO);
		if (VBO)
			glDeleteFramebuffers(1, &VBO);
	}

	// Generate VAO, VBO and EBO from mesh
	void Mesh::generateVAO()
	{
		// VAO initialization and binding
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO initialization and binding
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, attributs.size() * sizeof(float), attributs.data(), GL_STATIC_DRAW);

		int stride = 3 * sizeof(Core::Maths::vec3);

		// Set the attrib pointer to the positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(0));
		glEnableVertexAttribArray(0);

		// Set the attrib pointer to the texture coordinates
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(Core::Maths::vec3)));
		glEnableVertexAttribArray(1);

		// Set the attrib pointer to the normals
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(2 * sizeof(Core::Maths::vec3)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		isDrawable = true;

		Core::Engine::Benchmark::addTrianglesCount(verticesCount / 3.0);
	}

	void Mesh::compute(std::vector<Core::Maths::vec3>& vertices, std::vector<Core::Maths::vec3>& texCoords, std::vector<Core::Maths::vec3>& normals, std::vector<unsigned int>& indices)
	{
		// Create attributs vector from mesh values
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			Core::Maths::vec3& vertex = vertices[indices[i]];
			attributs.push_back(vertex.x);
			attributs.push_back(vertex.y);
			attributs.push_back(vertex.z);

			if (!texCoords.empty())
			{
				Core::Maths::vec3& textureCoords = texCoords[indices[i + 1]];
				attributs.push_back(textureCoords.x);
				attributs.push_back(textureCoords.y);
				attributs.push_back(textureCoords.z);
			}

			Core::Maths::vec3& normal = normals[indices[i + 2]];
			attributs.push_back(normal.x);
			attributs.push_back(normal.y);
			attributs.push_back(normal.z);

			verticesCount++;
		}
	}
}