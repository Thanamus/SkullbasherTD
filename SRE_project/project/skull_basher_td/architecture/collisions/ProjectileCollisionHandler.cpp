
#include "CustomCollisionHandler.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "ProjectileCollisionHandler.hpp"
// #include "../health/CrystalHealth.hpp"

// Sound Effects _Include
#include "../sound/SourceManager.hpp"
#include "../TransformComponent.hpp"
#include "../EnemyComponent.hpp"
#include "../health/CrystalHealth.hpp"

class GameObject;

ProjectileCollisionHandler::ProjectileCollisionHandler(GameObject *gameObject) : Component(gameObject) {}

void ProjectileCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
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

void ProjectileCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {
}

float ProjectileCollisionHandler::getDamage() const {
    return damage;
}

void ProjectileCollisionHandler::setDamage(float damage) {
    ProjectileCollisionHandler::damage = damage;
}
