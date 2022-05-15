#include "transform.hpp"

#include "imgui.h"

#include "utils.hpp"

namespace Physics
{
	Transform::Transform(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<Transform>(this))
	{
	}

	void Transform::onDestroy()
	{
		if (hasParent())
			getParent()->deleteChildFromTransform(this);
	}

	bool Transform::hasParent()
	{
		return parent != nullptr;
	}

	bool Transform::hasChild()
	{
		return children.size() > 0;
	}

	Transform* Transform::getChild(int childIndex)
	{
		return children[childIndex];
	}

	Engine::GameObject& Transform::getGOChild(int childIndex)
	{
		return children[childIndex]->getHost();
	}

	int Transform::getChildrenCount()
	{
		return (int)(children.size());
	}

	Engine::GameObject& Transform::getGOParent()
	{
		return parent->getHost();
	}

	std::shared_ptr<Physics::Transform> Transform::getParent()
	{
		return parent;
	}

	Core::Maths::mat4 Transform::getModel()
	{
		return Core::Maths::translate(m_position) *
			Core::Maths::rotateZ(m_rotation.z) *
			Core::Maths::rotateY(m_rotation.y) *
			Core::Maths::rotateX(m_rotation.x) *
			Core::Maths::scale(m_scale);
	}

	Core::Maths::mat4 Transform::getGlobalModel()
	{
		if (parent)
			return getParentModel() * getModel();

		return getModel();
	}

	Core::Maths::mat4 Transform::getParentModel() const
	{
		if (parent)
			return parent->getGlobalModel();

		return Core::Maths::identity();
	}

	void Transform::deleteChildFromTransform(Transform* transform)
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i] == transform)
			{
				children[i] = children.back();
				children.pop_back();

				return;
			}
		}
	}

	Core::Maths::vec3 Transform::getGlobalRotation() const
	{
		return m_rotation + getParentRotation();
	}

	Core::Maths::vec3 Transform::getGlobalPosition() const
	{
		return m_position + getParentPosition();
	}

	Core::Maths::vec3 Transform::getParentRotation() const
	{
		if (parent)
			return parent->getGlobalRotation();

		return Core::Maths::vec3(0.f, 0.f, 0.f);
	}

	Core::Maths::vec3 Transform::getParentPosition() const
	{
		if (parent)
			return parent->getGlobalPosition();

		return Core::Maths::vec3(0.f, 0.f, 0.f);
	}

	Core::Maths::vec3 Transform::getForward()
	{
		auto model = getGlobalModel();

		return -Core::Maths::vec3(model.e[2], model.e[6], model.e[10]);
	}

	void Transform::setParent(std::shared_ptr<Physics::Transform> _parent)
	{
		parent = _parent;
	}

	void Transform::setParent(Engine::GameObject& gameObject)
	{
		std::shared_ptr<Transform> newParent;
		if (!gameObject.tryGetComponent<Transform>(newParent))
			return;

		parent = newParent;
	}

	void Transform::setChild(Physics::Transform* child)
	{
		children.push_back(child);
	}

	void Transform::setChild(Engine::GameObject& gameObject)
	{
		std::shared_ptr<Transform> newParent;
		if (!gameObject.tryGetComponent<Transform>(newParent))
			return;

		children.push_back(newParent.get());
	}

	void Transform::update()
	{
		m_hasBeenUpdated = true;

		//m_rotation = Utils::clampLoop(m_rotation, 0.f, Core::Maths::TAU);
	}

	void Transform::drawImGui()
	{
		if (ImGui::TreeNode("Transform"))
		{
			Core::Maths::vec3 rotateDegrees = m_rotation * Core::Maths::RAD2DEG;

			ImGui::DragFloat3("Position :", &m_position.x);
			ImGui::DragFloat3("Rotation :", &rotateDegrees.x, 1.f, 0.f, 360.f);
			ImGui::DragFloat3("Scale :", &m_scale.x);

			m_rotation = rotateDegrees * Core::Maths::DEG2RAD;

			if (parent)
			{
				std::string parentName = "Parent: " + parent->getHost().m_name;
				ImGui::Text(parentName.c_str());
			}

			Component::drawImGui();

			ImGui::TreePop();
		}

		m_hasBeenUpdated = false;
	}

	std::string Transform::toString() const
	{
		return  "COMP TRANSFORM " + Utils::vecToStringParsing(m_position) + 
									Utils::vecToStringParsing(m_rotation) + 
									Utils::vecToStringParsing(m_scale) + (parent ? parent->getHost().m_name : "none");
	}

	void Transform::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss, std::string& parentName)
	{
		std::shared_ptr<Transform> transform;
		if (!gameObject.tryGetComponent(transform))
			transform = gameObject.addComponent<Transform>();

		iss >> transform->m_position.x;
		iss >> transform->m_position.y;
		iss >> transform->m_position.z;

		iss >> transform->m_rotation.x;
		iss >> transform->m_rotation.y;
		iss >> transform->m_rotation.z;

		iss >> transform->m_scale.x;
		iss >> transform->m_scale.y;
		iss >> transform->m_scale.z;

		iss >> parentName;
	}
}
