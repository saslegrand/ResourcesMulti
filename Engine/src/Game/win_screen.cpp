#include "win_screen.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"
#include "time.hpp"
#include "button.hpp"

namespace Gameplay
{
	WinScreen::WinScreen(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<WinScreen>(this))
	{

	}

	void WinScreen::start()
	{
	}

	void WinScreen::showWinScreen(bool isActive)
	{
		Core::Engine::Graph::setCursorState(isActive);

		for (int i = 0; i < 3; ++i)
			buttons[i]->setActive(isActive);
	}

	void WinScreen::drawImGui()
	{
		if (ImGui::TreeNode("WinScreen"))
			ImGui::TreePop();
	}

	std::string WinScreen::toString() const
	{
		return "COMP WINSCREEN";
	}

	void WinScreen::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<WinScreen>())
			gameObject.addComponent<WinScreen>();
	}
}