
#include "CustomCollisionHandler.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "CoinCollisionHandler.hpp"
#include "../PersonController.hpp"

// Sound Effects _Include
#include "../sound/SourceManager.hpp"
#include "../Transform.hpp"
#include "../CoinComponent.hpp"
#include "../../GameManager.hpp"

class GameObject;

CoinCollisionHandler::CoinCollisionHandler(GameObject *gameObject) : Component(gameObject) {}

void CoinCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if(gameObject->isQueuedForDeletion() || stopCollisions)
        return;

    if (begin ) {

        auto player = other->getComponent<PersonController>();
        if(player)
        {
            stopCollisions = true;
            gameObject->setQueuedForDeletion(true);
            GameManager::getInstance().addScore(gameObject->getComponent<CoinComponent>()->getMoney());
            SourceManager::Get()->playMyJam_global("coin.wav");
        }
    }
}

void CoinCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {
//    std::cout << "arrow collision end!" << std::endl;
}