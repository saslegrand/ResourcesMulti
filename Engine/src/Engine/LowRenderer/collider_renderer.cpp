#include "collider_renderer.hpp"

#include "imgui.h"

#include "resources_manager.hpp"

namespace LowRenderer
{
	ColliderRenderer::ColliderRenderer(Engine::GameObject& gameObject, std::shared_ptr<Physics::Collider> ptr, const std::string& modelFilePath)
		: Renderer(gameObject, std::shared_ptr<ColliderRenderer>(this), "colliderShader", false), collider(ptr), model(modelFilePath)
	{
	}

	void ColliderRenderer::draw() const
	{
		Core::Maths::mat4 newModel = getModelCollider();
		model.drawCollider(m_shaderProgram, newModel);
	}

	Core::Maths::mat4 ColliderRenderer::getModelCollider() const
	{
		return Core::Maths::translate(collider->m_center) *
			   Core::Maths::rotateX(m_transform->m_rotation.x) *
			   Core::Maths::rotateY(m_transform->m_rotation.y) *
			   Core::Maths::rotateZ(m_transform->m_rotation.z) *
			   Core::Maths::scale(collider->extensions);
	}

	bool ColliderRenderer::canBeDraw()
	{
		return collider->isDraw;
	}

	void ColliderRenderer::drawImGui()
	{
		if (ImGui::TreeNode("Collider renderer"))
		{
			Component::drawImGui();

			ImGui::Checkbox("IsDraw", &collider->isDraw);

			ImGui::TreePop();
		}
	}
}