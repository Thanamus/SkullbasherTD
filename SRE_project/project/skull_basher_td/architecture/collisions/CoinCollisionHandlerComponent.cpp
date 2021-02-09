
#include "CustomCollisionHandlerComponent.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBodyComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "CoinCollisionHandlerComponent.hpp"
#include "../PersonControllerComponent.hpp"

// Sound Effects _Include
#include "../sound/SourceManager.hpp"
#include "../TransformComponent.hpp"
#include "../CoinComponent.hpp"
#include "../../GameManager.hpp"

class GameObject;

/*
    coin collision handler processes collisions on the coin game objects
*/

CoinCollisionHandlerComponent::CoinCollisionHandlerComponent(GameObject *gameObject) : Component(gameObject) {}

void CoinCollisionHandlerComponent::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if(gameObject->isQueuedForDeletion() || stopCollisions)
        return; // check if the object is going to be deleted (means a collision has already happened and the player shouldn't get more money)

    if (begin ) {
        
        // if the other object is the person controller
        auto player = other->getComponent<PersonControllerComponent>();
        if(player)
        {
            stopCollisions = true; // so the player only gets money once (even if there are mulitple contact points registered in bullet)
            gameObject->setQueuedForDeletion(true);
            GameManager::getInstance().addScore(gameObject->getComponent<CoinComponent>()->getMoney()); // pay the player
            SourceManager::Get()->playMyJam_global("coin.wav"); // play the coin sound
        }
    }
}

void CoinCollisionHandlerComponent::onCollisionEnd(size_t collisionId, GameObject *other) {

}