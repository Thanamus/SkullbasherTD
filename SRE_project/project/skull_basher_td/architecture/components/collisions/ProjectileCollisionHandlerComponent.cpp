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

// on collision, damages enemies and queues the gameobject for deletion
void ProjectileCollisionHandlerComponent::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    // if allowed to collide
    if (begin && !stopCollisions) {
        auto enemy = other->getComponent<EnemyComponent>();
        // if it's an enemy
        if (enemy) {
            // and the enemy is hittable
            if(!enemy->isHittable())
                return;
            // prevent further collisions (especially important with box-box collisions)
            stopCollisions = true;
            // damage enemy
            enemy->decreaseHealth(damage);
            // queue gameObject for deletion
            gameObject->setQueuedForDeletion(true);
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
