#pragma once

#include "CollisionHandler.hpp"
#include "../Component.hpp"

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBodyComponent;

/*
 Collision handler that gets attached to 'enemy' game objects
*/

class EnemyCollisionHandlerComponent : public Component, public CollisionHandler {
public:
    explicit EnemyCollisionHandlerComponent(GameObject* gameObject);

    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;
    void onCollisionEnd(size_t collisionId, GameObject *other) override;

};