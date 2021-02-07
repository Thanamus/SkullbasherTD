//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Targetable.hpp"
#include "Updatable.hpp"
#include "GameObject.hpp"

class HomingComponent : public Component, public Updatable, Targetable {
public:
    explicit HomingComponent(GameObject* gameObject_);
    void update(float deltaTime) override;

    float getSpeed() const;
    void setSpeed(float speed);

private:
    float speed = 0;
};