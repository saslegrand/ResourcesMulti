#include "main_menu.hpp"

#include <algorithm> 
#include <imgui.h>

#include "inputs_manager.hpp"
#include "application.hpp"
#include "graph.hpp"

#include "button.hpp"

namespace Gameplay
{
	MainMenu::MainMenu(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<MainMenu>(this))
	{

	}

	void MainMenu::start()
	{
		Core::Engine::Graph::setCursorState(true);

		/*Engine::GameObject* goButtonLoadScene = Core::Engine::Graph::findGameObjectWithName("LoadSceneButton");
		std::shared_ptr<UI::Button> buttonLoadScene = goButtonLoadScene->getComponent<UI::Button>();
		UI::Button* loadScenePtr = buttonLoadScene.get();
		loadScenePtr->addListener(UI::ButtonState::DOWN, [](){
			Core::Engine::Graph::loadSaveGame();
		});
		loadScenePtr->addListener(UI::ButtonState::HIGHLIGHT, [loadScenePtr]() {
			loadScenePtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
		});*/

		/*Engine::GameObject* goButtonOptions = Core::Engine::Graph::findGameObjectWithName("OptionsButton");
		std::shared_ptr<UI::Button> buttonOptions = goButtonOptions->getComponent<UI::Button>();
		UI::Button* optionsPtr = buttonOptions.get();
		optionsPtr->addListener(UI::ButtonState::DOWN, [](){
			Core::Debug::Log::info("Options");
		});
		optionsPtr->addListener(UI::ButtonState::HIGHLIGHT, [optionsPtr]() {
			optionsPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
		});*/

		Engine::GameObject* goButtonExit = Core::Engine::Graph::findGameObjectWithName("ExitButton");
		std::shared_ptr<UI::Button> buttonExit = goButtonExit->getComponent<UI::Button>();
		UI::Button* exitPtr = buttonExit.get();
		exitPtr->addListener(UI::ButtonState::DOWN, [](){
			Core::Application::closeApplication();
		});
		exitPtr->addListener(UI::ButtonState::HIGHLIGHT, [exitPtr]() {
			exitPtr->getSprite()->m_color = Core::Maths::vec4(0.8f, 0.3f, 0.3f, 1.f);
		});
	}

	void MainMenu::update()
	{
	}

	void MainMenu::drawImGui()
	{
		if (ImGui::TreeNode("MainMenu"))
		{
			
			ImGui::TreePop();
		}
	}

	std::string MainMenu::toString() const
	{
		return "COMP MAINMENU";
	}

	void MainMenu::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		if (!gameObject.tryGetComponent<MainMenu>())
			gameObject.addComponent<MainMenu>();
	}
}