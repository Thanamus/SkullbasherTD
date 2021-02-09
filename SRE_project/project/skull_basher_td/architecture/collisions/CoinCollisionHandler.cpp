

#include "../GameObject.hpp"

#include "CoinCollisionHandler.hpp"
#include "../PersonController.hpp"

// Sound Effects _Include
#include "../sound/SourceManager.hpp"
#include "../Transform.hpp"
#include "../CoinComponent.hpp"
#include "../../GameManager.hpp"

class GameObject;

/*
    coin collision handler processes collisions on the coin game objects
*/

CoinCollisionHandler::CoinCollisionHandler(GameObject *gameObject) : Component(gameObject) {}

void CoinCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if(gameObject->isQueuedForDeletion() || stopCollisions)
        return; // check if the object is going to be deleted (means a collision has already happened and the player shouldn't get more money)

    if (begin ) {
        
        // if the other object is the person controller
        auto player = other->getComponent<PersonController>();
        if(player)
        {
            stopCollisions = true; // so the player only gets money once (even if there are mulitple contact points registered in bullet)
            gameObject->setQueuedForDeletion(true);
            GameManager::getInstance().addScore(gameObject->getComponent<CoinComponent>()->getMoney()); // pay the player
            SourceManager::Get()->playMyJam_global("coin.wav"); // play the coin sound
        }
    }
}

void CoinCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {

}