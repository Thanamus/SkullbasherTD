#pragma once

#include "../../mixins/CollisionHandler.hpp"
#include "../Component.hpp"

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBodyComponent;

/*
    coin collision handler processes collisions on the coin game objects
*/


class CoinCollisionHandlerComponent : public Component, public CollisionHandler {
public:
    explicit CoinCollisionHandlerComponent(GameObject *gameObject);

    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;

    void onCollisionEnd(size_t collisionId, GameObject *other) override;
};