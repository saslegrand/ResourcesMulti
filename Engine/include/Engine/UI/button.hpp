#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

#include "sprite_renderer.hpp"

namespace Physics
{
	class Transform;
}

namespace UI
{
	enum class ButtonState
	{
		HIGHLIGHT,
		DOWN,
		STAY,
		UP
	};

	class Button : public Engine::Component
	{
	private:
		std::shared_ptr<LowRenderer::SpriteRenderer> m_image = nullptr;
		std::shared_ptr<Physics::Transform> m_transform = nullptr;
		std::unordered_map<ButtonState, std::vector<std::function<void()>>> functions;

		void onClick();
		void onHighlight();
		void onClickStay();
		void onClickRelease();

	public:
		Button(Engine::GameObject& gameObject, const std::string& shaderProgramName, const std::string& texturePath);

		void update() override;

		void addListener(ButtonState state, std::function<void()> function);

		void drawImGui() override;
		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);

		std::shared_ptr<LowRenderer::SpriteRenderer> getSprite();
	};
}
