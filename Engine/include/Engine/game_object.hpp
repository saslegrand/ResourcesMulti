#pragma once

#include <vector>
#include <string>
#include <memory>
#include <istream>

#include "object.hpp"
#include "collision.hpp"

#include "debug.hpp"

namespace Physics
{
	class Collider;
}

namespace Engine
{
	class Component;

	class GameObject : public Object
	{
	private:

	public:
		bool isStatic = false;

		std::string m_name = "GameObject";

		std::string m_recipe = "";

		std::vector<std::shared_ptr<Component>> m_components;

		GameObject() = default;
		GameObject(const std::string& name);
		virtual ~GameObject();

		template <class C, typename ...Args, typename Base = std::enable_if_t<std::is_base_of<Component, C>::value>>
		std::shared_ptr<C> addComponent(Args... args)
		{
			new C(*this, args...);

			return getComponent<C>();
		}

		template <class C, typename Base = std::enable_if_t<std::is_base_of<Component, C>::value>>
		bool tryGetComponent()
		{
			std::shared_ptr<C> componentToReturn;
			return tryGetComponent(componentToReturn);
		}

		template <class C, typename Base = std::enable_if_t<std::is_base_of<Component, C>::value>>
		bool tryGetComponent(std::shared_ptr<C>& componentToReturn)
		{
			for (std::shared_ptr<Component>& component : m_components)
			{
				auto castedComponent = std::dynamic_pointer_cast<C>(component);

				if (!castedComponent)
					continue;

				componentToReturn = castedComponent;
				return true;
			}

			return false;
		}

		template <class C, typename Base = std::enable_if_t<std::is_base_of<Component, C>::value>>
		[[nodiscard]] std::shared_ptr<C> getComponent()
		{
			std::shared_ptr<C> componentToReturn;
			
			bool hasComponent = tryGetComponent(componentToReturn);

			//if (!hasComponent)
				//Core::Debug::Log::error("Component not found");

			return componentToReturn;
		}

		void awakeComponents();
		void updateComponents();
		void fixedUpdateComponents();
		void lateUpdateComponents();

		void callCollisionEnter(const Physics::Collision& collision);
		void callCollisionStay(const Physics::Collision& collision);
		void callCollisionExit(const Physics::Collision& collision);

		void callTriggerEnter(Physics::Collider* collider);
		void callTriggerStay(Physics::Collider* collider);
		void callTriggerExit(Physics::Collider* collider);

		void drawImGuiInspector();
		void drawImGuiHierarchy(std::string& curDrawGoName, bool isDrawFromScene);

		std::string toString();

		void parseComponents(std::istringstream& parseComponent, std::string& parentName, bool isMultiThreaded);
		void parseRecipe(const std::string& filePath, std::string& parentName);
		void parse(std::istream& scnStream, std::string& parentName, bool isMultiThreaded);

		void destroy() override;
		void onDestroy() override;
	};

}