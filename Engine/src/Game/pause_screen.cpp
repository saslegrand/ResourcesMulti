#include "pause_screen.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"
#include "time.hpp"
#include "button.hpp"

namespace Gameplay
{
	PauseScreen::PauseScreen(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PauseScreen>(this))
	{

	}

	void PauseScreen::start()
	{
	}

	void PauseScreen::showPauseScreen(bool isActive)
	{
		Core::Engine::Graph::setCursorState(isActive);

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(isActive);
	}

	void PauseScreen::drawImGui()
	{
		if (ImGui::TreeNode("PauseScreen"))
			ImGui::TreePop();
	}

	std::string PauseScreen::toString() const
	{
		return "COMP PAUSESCREEN";
	}

	void PauseScreen::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<PauseScreen>())
			gameObject.addComponent<PauseScreen>();
	}
}