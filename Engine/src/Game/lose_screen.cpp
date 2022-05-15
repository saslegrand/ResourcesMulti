#include "lose_screen.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"
#include "time.hpp"
#include "button.hpp"

namespace Gameplay
{
	LoseScreen::LoseScreen(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<LoseScreen>(this))
	{

	}

	void LoseScreen::start()
	{
	}

	void LoseScreen::showLoseScreen(bool isActive)
	{
		Core::Engine::Graph::setCursorState(isActive);

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(isActive);
	}

	void LoseScreen::drawImGui()
	{
		if (ImGui::TreeNode("LoseScreen"))
			ImGui::TreePop();
	}

	std::string LoseScreen::toString() const
	{
		return "COMP LOSESCREEN";
	}

	void LoseScreen::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<LoseScreen>())
			gameObject.addComponent<LoseScreen>();
	}
}