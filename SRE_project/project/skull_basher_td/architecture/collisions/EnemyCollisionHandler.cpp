
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

EnemyCollisionHandler::~EnemyCollisionHandler(){

}


void EnemyCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    // std::cout << "EnemyCollisionHandler "  <<std::endl;
    
    if (begin){
        auto crystal = other->getComponent<CrystalHealth>();
        std::cout << "crystal " << crystal <<std::endl;
        if(crystal != nullptr)
        {
            std::cout << "hit crystal "<<std::endl;
            crystal->decreaseHealth(25);
            gameObject->deleteMe = true;
        }
    }
}

void EnemyCollisionHandler::onCollisionEnd(size_t collisionId) {
    std::cout << "Collision end "<<collisionId<<std::endl;
}