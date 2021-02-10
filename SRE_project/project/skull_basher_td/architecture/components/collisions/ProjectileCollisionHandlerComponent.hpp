#pragma once

#include "../../mixins/CollisionHandler.hpp"
#include "../Component.hpp"

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBodyComponent;

class ProjectileCollisionHandlerComponent : public Component, public CollisionHandler {
public:
    // explicit CustomCollisionHandlerComponent(GameObject *gameObject) : Component(gameObject));
    explicit ProjectileCollisionHandlerComponent(GameObject *gameObject);

    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;

    void onCollisionEnd(size_t collisionId, GameObject *other) override;

    float getDamage() const;

    void setDamage(float damage);

private:
    float damage = 1.f;
};