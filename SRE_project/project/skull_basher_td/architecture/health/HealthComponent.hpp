#pragma once

#include "../Component.hpp"


#define GLM_ENABLE_EXPERIMENTAL


class HealthComponent : public Component  {
public:
    explicit HealthComponent(GameObject* gameObject);

    void debugGUI() override ;

    virtual ~HealthComponent();

    float getHealth();
    void setHealth(float health);
    void addHealth(float health);
    void decreaseHealth(float health);
    virtual void HandleHealthChange();

protected:
    float health = 0.0f;
};

