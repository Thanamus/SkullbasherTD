#pragma once

#include "Component.hpp"

class BirdController : public Component {
public:
    explicit BirdController(GameObject *gameObject);

    void periodicSpeedUp(float deltaTime);

    bool onKey(SDL_Event &event) override;

    void onCollisionStart(PhysicsComponent *comp) override;

    void onCollisionEnd(PhysicsComponent *comp) override;

    void jump();

    bool jumped = false; // space is used to start the game, so to avoid dying immediately i set it as if it was jumping
};
