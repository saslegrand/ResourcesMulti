#include "sub_model.hpp"

#include "transform.hpp"
#include "resources_manager.hpp"

namespace LowRenderer
{
	SubModel::SubModel(const std::string& meshName)
		: m_mesh(Resources::ResourcesManager::getMeshByName(meshName))
	{

	}

	SubModel::SubModel(const std::shared_ptr<Resources::Mesh> mesh, const std::shared_ptr<Resources::Material> material)
		: m_mesh(mesh), m_material(material) { }

	void SubModel::generate()
	{
		// generate mesh
		m_mesh->generateVAO();

		// generate textures
		m_material->alphaTex->generate();
		m_material->ambientTex->generate();
		m_material->diffuseTex->generate();
		m_material->specularTex->generate();
		m_material->emissiveTex->generate();
	}

	void SubModel::setMaterial(std::shared_ptr<Resources::Material>& mat)
	{
		m_material = mat;
	}

	void SubModel::draw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> transform) const
	{
		if (m_mesh->isDrawable)
		{
			// Send model matrix to program
			shaderProgram->setUniform("model", transform->getGlobalModel().e, 1, 1);

			std::shared_ptr<Resources::Material> mat = Resources::Material::defaultMaterial;

			if (m_material->isDrawable)
				mat = m_material;

			mat->sendToShader(shaderProgram);
			mat->bindTextures();

			// Draw the mesh
			m_mesh->draw();
		}
	}

	void SubModel::simpleDraw(std::shared_ptr<Resources::ShaderProgram> shaderProgram, std::shared_ptr<Physics::Transform> transform) const
	{
		if (m_mesh->isDrawable)
		{
			shaderProgram->setUniform("model", transform->getGlobalModel().e, 1, 1);

			// Draw the mesh
			m_mesh->draw();
		}
	}

	void SubModel::drawCollider(std::shared_ptr<Resources::ShaderProgram> shaderProgram, Core::Maths::mat4& modelCollider) const
	{

	}
}