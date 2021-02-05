
#pragma once
#include "../Component.hpp"
#include "../Updatable.hpp"

#include "glm/glm.hpp"
#include <chrono>

class GameObject;
/*
    ArrowLifespanComponent is a simple component to help manage a gameObject lifespan
    Although not as ideal as a pool, it does help cut down the number of objects 
    needing updates (especially physics updates)
*/


class ArrowLifespanComponent : public Component, public Updatable {
public:
    explicit ArrowLifespanComponent(GameObject *gameObject);
    ~ArrowLifespanComponent();

    void update(float deltaTime) override;

    virtual void setLifespan(int life_span); // sets the lifespan of the object

    int getLifespan();

private:
    std::chrono::steady_clock::time_point start_life; // time point lifespan starts
    int lifespan_millisec = 10000; // how long the obect will live in millisec, default is 10 sec
};
