#pragma once

#include "collider.hpp"
#include "box.h"

namespace Physics
{
	class BoxCollider : public Collider
	{
	private:
		BoxCollider(Engine::GameObject& gameObject, std::shared_ptr<BoxCollider> ptr);

		void onDestroy() override;
	public:
		BoxCollider(Engine::GameObject& gameObject);

		Box box = Box();

		void updateShape() override;

		void drawImGui() override;

		std::string toString() const override;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}