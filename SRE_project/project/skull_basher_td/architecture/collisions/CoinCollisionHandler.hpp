#pragma once

#include "CollisionHandler.hpp"
#include "../Component.hpp"

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBody;

/*
    coin collision handler processes collisions on the coin game objects
*/


class CoinCollisionHandler : public Component, public CollisionHandler {
public:
    explicit CoinCollisionHandler(GameObject *gameObject);

    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;

    void onCollisionEnd(size_t collisionId, GameObject *other) override;
};