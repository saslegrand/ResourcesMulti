#include "model.hpp"

#include "imgui.h"

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "thread_pool.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Model::Model(const std::string& filePath)
		: m_filePath(filePath)
	{
	}

	//rayane est passe par la
	void Model::addSubModel(std::shared_ptr<Resources::Mesh> mesh, std::shared_ptr<Resources::Material> material)
	{
		mesh->generateVAO();
		m_subModels.push_back(SubModel(mesh, material));
	}

	void Model::test(const std::string& filePath)
	{
		m_filePath = filePath;
	}

	void Model::draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> transform) const
	{
		for (size_t i = 0; i < m_subModels.size(); i++)
			m_subModels[i].draw(shaderProgram, transform);
	}

	void Model::simpleDraw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> transform) const
	{
		// Draw children
		for (const SubModel& subM : m_subModels)
			subM.simpleDraw(shaderProgram, transform);
	}

	void Model::drawCollider(std::shared_ptr<Resources::ShaderProgram> shaderProgram, Core::Maths::mat4& modelCollider) const
	{
		/*if (m_mesh)
		{
			Core::Maths::vec3 color = Core::Maths::vec3(0.f, 1.f, 0.f);
			// Send model matrix to program
			shaderProgram->setUniform("model", modelCollider.e, 1, 1);
			shaderProgram->setUniform("color", color.e, 1, 1);

			// Draw the mesh
			m_mesh->draw();
		}

		// Draw children
		for (const Model& child : m_children)
			child.drawCollider(shaderProgram, modelCollider);*/
	}

	std::string Model::getPath() const
	{
		return m_filePath;
	}

	void Model::drawImGui()
	{
		ImGui::Text(m_filePath.c_str());
	}

	void Model::setDiffuseTexture(const std::string& difTexName)
	{
		
	}
}