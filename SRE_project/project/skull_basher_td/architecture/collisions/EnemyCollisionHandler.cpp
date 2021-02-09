
#include "CustomCollisionHandler.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "EnemyCollisionHandler.hpp"
#include "../health/CrystalHealth.hpp"
#include "../../GameManager.hpp"

class GameObject;

EnemyCollisionHandler::EnemyCollisionHandler(GameObject* gameObject) : Component(gameObject) {

}

void EnemyCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if (begin && !stopCollisions){
        auto crystal = other->getComponent<CrystalHealth>();
//        std::cout << "crystal " << crystal <<std::endl;
        if(crystal != nullptr)
        {
            stopCollisions = true;
            gameObject->setQueuedForDeletion(true);
            std::cout << "hit crystal "<<std::endl;
            crystal->decreaseHealth(getGameObject()->getComponent<EnemyComponent>()->getDamage());
            GameManager::getInstance().setTotalEnemies(GameManager::getInstance().getTotalEnemies() - 1);
            if(GameManager::getInstance().getTotalEnemies() <= 0)
            {
                GameManager::getInstance().toggleWinState(true);
            }
        }
    }
}

void EnemyCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {
}