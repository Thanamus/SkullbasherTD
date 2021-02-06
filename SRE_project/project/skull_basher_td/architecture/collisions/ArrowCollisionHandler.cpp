
#include "CustomCollisionHandler.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "ArrowCollisionHandler.hpp"
// #include "../health/CrystalHealth.hpp"

// Sound Effects _Include
#include "../sound/SourceManager.hpp"
#include "../Transform.hpp"
#include "../EnemyComponent.hpp"
#include "../health/CrystalHealth.hpp"

class GameObject;

ArrowCollisionHandler::ArrowCollisionHandler(GameObject *gameObject) : Component(gameObject) {}

void ArrowCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if (begin) {
        /* TODO ask andrea what is a better coding pattern
         auto soundeffect = SourceManager::Get(); // get sound effect player
        soundeffect->playMyJam("deathd.wav", otherObjectsPosition, 20);

        OR
        SourceManager::Get()->playMyJam("deathd.wav", otherObjectsPosition, 20);
        */
        // auto soundeffect = SourceManager::Get(); // get sound effect player

        auto enemy = other->getComponent<EnemyComponent>();
        if (enemy) {
            if (canInjure)
            {
                /* code */
                std::cout << "bashing skulls for days" << std::endl;
                enemy->decreaseHealth(1);
                canInjure = false; // box-box collisions create multiple contact points -> kills enemy fast
            }
            
        }
    }
}

void ArrowCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {
//    std::cout << "arrow collision end!" << std::endl;
    // auto enemy = other->getComponent<EnemyComponent>();
    // if(enemy)
    //     gameObject->deleteMe = true;
}