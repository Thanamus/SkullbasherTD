
#include "CustomCollisionHandler.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "EnemyCollisionHandler.hpp"
#include "../health/CrystalHealth.hpp"

class GameObject;

EnemyCollisionHandler::EnemyCollisionHandler(GameObject* gameObject) : Component(gameObject) {

}

void EnemyCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if (begin){
        std::cout << "EnemyCollisionHandler collided with "  << other->getComponent<RigidBody>()->getGroupID() << std::endl;
        auto crystal = other->getComponent<CrystalHealth>();
//        std::cout << "crystal " << crystal <<std::endl;
        if(crystal != nullptr)
        {
            std::cout << "hit crystal "<<std::endl;
            crystal->decreaseHealth(25);
        }
    }
}

void EnemyCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {
//    std::cout << "Collision end "<<collisionId<<std::endl;
    if(other->getComponent<RigidBody>()->getGroupID() == CRYSTAL)
        gameObject->deleteMe = true;
}