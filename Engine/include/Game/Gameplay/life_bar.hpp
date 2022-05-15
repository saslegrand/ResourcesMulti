#pragma once

#include <memory>

#include "component.hpp"
#include "transform.hpp"

namespace Gameplay
{
    class LifeBar : public Engine::Component
    {
    private:
        Physics::Transform* pivot = nullptr;

    public:
        LifeBar(Engine::GameObject& gameObject);

        void start() override;
        void updateSprite(int life, int maxLife);

        std::string toString() const;

        static void parseComponent(Engine::GameObject& gameObject, std::istringstream& iss);
    };
}