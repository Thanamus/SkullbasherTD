
#pragma once
#include "GameObject.hpp"
#include "Component.hpp"
#include "Updatable.hpp"

#include "glm/glm.hpp"
#include <chrono>

/*
    Expirable is a simple component to help manage a gameObject lifespan
    Although not as ideal as a pool, it does help cut down the number of objects 
    needing updates (especially physics updates)
*/


class Expirable : public Component, public Updatable
{
public:
    explicit Expirable(GameObject *gameobject);
    ~Expirable();

    void update(float deltaTime) override;

    void setLifespan(int life_span); // sets the lifespan of the object

    int getLifespan();

private:
    std::chrono::steady_clock::time_point start_life; // time point lifespan starts
    int lifespan_millisec = 10000; // how long the obect will live in millisec, default is 10 sec

};
