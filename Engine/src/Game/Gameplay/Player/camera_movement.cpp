#include "camera_movement.hpp"

#include <imgui.h>
#include <algorithm>

#include "inputs_manager.hpp"
#include "physic_manager.hpp"
#include "time.hpp"
#include "graph.hpp"
#include "maths.hpp"

#include "utils.hpp"

namespace Gameplay
{
	CameraMovement::CameraMovement(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<CameraMovement>(this))
	{
		camera = requireComponent<LowRenderer::Camera>();
		transform = requireComponent<Physics::Transform>();
	}

	void CameraMovement::start()
	{
		playerTransform = transform->getGOParent().getComponent<Physics::Transform>();
		bench = Core::Engine::Benchmark::getInstance();
	}

	void CameraMovement::fixedUpdate()
	{
		if (bench->isActive())
			return;

		transform->m_rotation.x -= m_sensitivity * Core::TimeManager::getFixedDeltaTime() * Core::Input::InputManager::getDeltasMouse().y;
		transform->m_rotation.x = std::clamp(transform->m_rotation.x, -Core::Maths::PIO2, Core::Maths::PIO2);
	}

	void CameraMovement::drawImGui()
	{
		if (ImGui::TreeNode("CameraMovement"))
		{
			ImGui::DragFloat("Sensitivity : ", &m_sensitivity);
			ImGui::Text("Arrows Left/Right : rotate horizontaly");
			ImGui::Text("Arrows  Down/Up   : rotate vertically");
			ImGui::TreePop();
		}
	}

	std::string CameraMovement::toString() const
	{
		return "COMP CAMERAMOVEMENT " + std::to_string(m_sensitivity);
	}

	void CameraMovement::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<CameraMovement>();
		auto player = gameObject.getComponent<CameraMovement>();

		iss >> player->m_sensitivity;
	}
}