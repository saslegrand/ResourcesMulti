#pragma once

#include "mesh.hpp"
#include "shader.hpp"
#include "material.hpp"

namespace Physics
{
	class Transform;
}

namespace LowRenderer
{
	class SubModel
	{
	private:
		std::shared_ptr<Resources::Mesh> m_mesh = nullptr;
		std::shared_ptr<Resources::Material> m_material = Resources::Material::defaultMaterial;

	public:
		SubModel(const std::string& meshName);
		SubModel(const std::shared_ptr<Resources::Mesh> mesh, const std::shared_ptr<Resources::Material> material);

		void generate();
		void setMaterial(std::shared_ptr<Resources::Material>& mat);

		void draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> tramsform) const;
		void simpleDraw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> transform) const;
		void drawCollider(std::shared_ptr<Resources::ShaderProgram> shaderProgram, Core::Maths::mat4& modelCollider) const;
	};
}