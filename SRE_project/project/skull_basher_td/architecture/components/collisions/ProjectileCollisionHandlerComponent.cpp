#include "../../scenes/GameObject.hpp"
#include "../physics/RigidBodyComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "ProjectileCollisionHandlerComponent.hpp"
// #include "../health/CrystalHealthComponent.hpp"

// Sound Effects _Include
#include "../../sound/SourceManager.hpp"
#include "../TransformComponent.hpp"
#include "../game_entities/enemies/EnemyComponent.hpp"
#include "../health/CrystalHealthComponent.hpp"

class GameObject;

ProjectileCollisionHandlerComponent::ProjectileCollisionHandlerComponent(GameObject *gameObject) : Component(gameObject) {}

void ProjectileCollisionHandlerComponent::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if (begin && !stopCollisions) {
        auto enemy = other->getComponent<EnemyComponent>();

        if (enemy) {
            if(!enemy->isHittable())
                return;
            stopCollisions = true;
            gameObject->setQueuedForDeletion(true);
            enemy->decreaseHealth(damage);
        }
    }
}

void ProjectileCollisionHandlerComponent::onCollisionEnd(size_t collisionId, GameObject *other) {
}

float ProjectileCollisionHandlerComponent::getDamage() const {
    return damage;
}

void ProjectileCollisionHandlerComponent::setDamage(float damage) {
    ProjectileCollisionHandlerComponent::damage = damage;
}
