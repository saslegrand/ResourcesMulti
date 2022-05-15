#include "model_renderer.hpp"

#include "imgui.h"

#include "resources_manager.hpp"
#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "benchmark.hpp"
#include "time.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	ModelRenderer::ModelRenderer(Engine::GameObject& gameObject, const std::shared_ptr<ModelRenderer>& ptr, const std::string& shaderPromgramName)
		: Renderer(gameObject, ptr, shaderPromgramName)
	{
		RenderManager::linkComponent(ptr);
	}

	ModelRenderer::ModelRenderer(Engine::GameObject& gameObject, const std::string& filePath, const std::string& shaderPromgramName, bool isMulti, const Core::Maths::vec2& tilling)
		: ModelRenderer(gameObject, std::shared_ptr<ModelRenderer>(this), shaderPromgramName)
	{
		model = Resources::ResourcesManager::loadObj(filePath, isMulti);

		tillingMultiplier = tilling.x;
		tillingOffset = tilling.y;

		Core::Engine::Benchmark::addObjectsCount(1);
	}

	ModelRenderer::~ModelRenderer()
	{
		Core::Debug::Log::info("Unload model " + model->getPath());
	}

	void ModelRenderer::onDestroy()
	{
		Component::onDestroy();

		LowRenderer::RenderManager::removeComponent(this);
	}

	void ModelRenderer::draw() const
	{
		m_shaderProgram->setUniform("tilling", Core::Maths::vec2(tillingMultiplier, tillingOffset).e);

		model->draw(m_shaderProgram, m_transform);
	}

	void ModelRenderer::simpleDraw(std::shared_ptr<Resources::ShaderProgram> program) const
	{
		model->simpleDraw(program, m_transform);
	}

	void ModelRenderer::drawImGui()
	{
		if (ImGui::TreeNode("Model renderer"))
		{
			ImGui::Text("Textures :");

			ImGui::DragFloat("TillingMultiplier", &tillingMultiplier, 0.1f, 0.f, 200.f);
			ImGui::DragFloat("TillingOffset", &tillingOffset, 0.01f, 0.f, 1.f);

			model->drawImGui();
			Component::drawImGui();

			ImGui::TreePop();
		}
	}

	std::string ModelRenderer::toString() const
	{
		return "COMP MODELRENDERER " + model->getPath() + " " + m_shaderProgram->getName() + " " + std::to_string(tillingMultiplier) + " " + std::to_string(tillingOffset);
	}

	void ModelRenderer::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss, bool isMultiThreaded)
	{
		std::string modelPath, shaderProgramName;
		Core::Maths::vec2 tilling;

		iss >> modelPath;
		iss >> shaderProgramName;
		iss >> tilling.x;
		iss >> tilling.y;

		gameObject.addComponent<ModelRenderer>(modelPath, shaderProgramName, isMultiThreaded, tilling);
	}
}