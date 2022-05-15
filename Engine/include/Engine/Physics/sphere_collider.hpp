#pragma once

#include "collider.hpp"
#include "sphere.h"

namespace Physics
{
	class SphereCollider : public Collider
	{
	private:
		SphereCollider(Engine::GameObject& gameObject, std::shared_ptr<SphereCollider> ptr);

		void onDestroy() override;
	public:
		SphereCollider(Engine::GameObject& gameObject);

		void updateShape() override;
		void drawImGui() override;

		std::string toString() const override;

		Sphere sphere;

		static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
	};
}