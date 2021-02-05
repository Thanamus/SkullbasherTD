
#pragma once
#include "GameObject.hpp"
#include "Component.hpp"
#include "Updatable.hpp"

#include "glm/glm.hpp"
#include <chrono>

class Expirable : public Component, public Updatable
{
public:
    explicit Expirable(GameObject *gameobject);
    ~Expirable();

    void update(float deltaTime) override;

    void setLifespan();

    int getLifespan();

private:
    std::chrono::steady_clock::time_point start_life;
    int lifespan_millisec = 10000;

};
