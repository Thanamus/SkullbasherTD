#pragma once

#include "CollisionHandler.hpp"
#include "../Component.hpp"

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBody;

class ArrowCollisionHandler : public Component, public CollisionHandler {
public:
    // explicit CustomCollisionHandler(GameObject *gameObject) : Component(gameObject));
    explicit ArrowCollisionHandler(GameObject *gameObject);

    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;

    void onCollisionEnd(size_t collisionId) override; 

};