#pragma once

#include "CollisionHandler.hpp"
#include "../Component.hpp"

#include <chrono>

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBody;

class PlayerCollisionHandler : public Component, public CollisionHandler {
public:
    explicit PlayerCollisionHandler(GameObject* gameObject);

    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;

    void onCollisionEnd(size_t collisionId, GameObject *other) override;
private:
    // Footstep sound timing
    std::chrono::steady_clock::time_point start_footstep_lockout;
    int footstep_lockout_millisec = 300;
};