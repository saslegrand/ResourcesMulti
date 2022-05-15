#pragma once

#include "renderer.hpp"
#include "model.hpp"

namespace LowRenderer
{
	class ModelRenderer : public Renderer
	{
	private:
		std::shared_ptr<LowRenderer::Model> model;

		float tillingMultiplier = 1.f;
		float tillingOffset = 0.f;

		void onDestroy() override;

	public:
		ModelRenderer(Engine::GameObject& gameObject, const std::shared_ptr<ModelRenderer>& ptr, const std::string& shaderPromgramName);
		ModelRenderer(Engine::GameObject& gameObject, const std::string& filePath, const std::string& shaderPromgramName, bool isMulti, const Core::Maths::vec2& tilling = Core::Maths::vec2(1.f, 0.f));
		~ModelRenderer();

		void draw() const override;
		void simpleDraw(std::shared_ptr<Resources::ShaderProgram> program) const;
		void drawImGui() override;
		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss, bool isMultiThreaded);
	};
}