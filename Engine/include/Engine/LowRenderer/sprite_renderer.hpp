#pragma once

#include "renderer.hpp"
#include "texture.hpp"
#include "mesh.hpp"

namespace LowRenderer
{
	class SpriteRenderer : public Renderer
	{
	private:
		std::shared_ptr<Resources::Mesh> mesh = nullptr;
		std::shared_ptr<Resources::Texture> texture = nullptr;

		float tillingMultiplier = 1.f;
		float tillingOffset = 0.f;

		void onDestroy() override;

	public:
		Core::Maths::vec4 m_color = Core::Maths::vec4(1.f, 1.f, 1.f, 1.f);

		SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName, const Core::Maths::vec2& tilling = Core::Maths::vec2(1.f, 0.f));
		SpriteRenderer(Engine::GameObject& gameObject, const std::string& shaderPromgramName, const std::string& texturePath, bool isMulti, const Core::Maths::vec2& tilling = Core::Maths::vec2(1.f, 0.f));
		SpriteRenderer(Engine::GameObject& gameObject, const std::shared_ptr<SpriteRenderer>& ptr, const std::string& shaderPromgramName);

		std::string getTexturePath();

		void draw() const override;
		void drawImGui() override;
		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss, bool isMulti);
	};
}