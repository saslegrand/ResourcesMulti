#pragma once

#include <memory>
#include <deque>
#include <atomic>

#include "sub_model.hpp"

#define COLLIDER_COLOR Core::Maths::vec3(0.f, 1.f, 0.f)

namespace Physics
{
	class Transform;
}

namespace LowRenderer
{
	class Model
	{
	private:
		std::string m_filePath;

	public:

		std::deque<SubModel> m_subModels;
		Model(const std::string& filePath);

		Model() = default;

		void test(const std::string& filePath);

		void addSubModel(std::shared_ptr<Resources::Mesh> mesh, std::shared_ptr<Resources::Material> material);

		void draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> transform) const;
		void simpleDraw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> transform) const;
		void drawCollider(std::shared_ptr<Resources::ShaderProgram> shaderProgram, Core::Maths::mat4& modelCollider) const;
		void drawImGui();

		void setDiffuseTexture(const std::string& difTexName);

		std::string getPath() const;
	};

	/*class Model
	{
	private:
		std::shared_ptr<Resources::Mesh> m_mesh = nullptr;
		std::shared_ptr<Resources::Material> m_material = Resources::Material::defaultMaterial;

		std::vector<Model> m_children;

		std::string m_filePath;

		Model(std::shared_ptr<Physics::Transform>& transform, const std::string& meshName);

	public:
		std::shared_ptr<Physics::Transform> m_transform = nullptr;

		Model(const std::string& filePath, std::shared_ptr<Physics::Transform> transform);

		Model() = default;

		void draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram) const;
		void simpleDraw(std::shared_ptr<Resources::ShaderProgram> shaderProgram) const;
		void drawCollider(std::shared_ptr<Resources::ShaderProgram> shaderProgram, Core::Maths::mat4& modelCollider) const;
		void drawImGui();

		void setDiffuseTexture(const std::string& difTexName);

		std::string getPath() const;
	};*/
}
