
#include "CustomerCollisionHandler.hpp"

#include "GameObject.hpp"
#include "RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "EnemyCollisionHandler.hpp"

class GameObject;

EnemyCollisionHandler::EnemyCollisionHandler(GameObject *gameObject) : Component(gameObject) {

}

EnemyCollisionHandler::~EnemyCollisionHandler(){

}


void EnemyCollisionHandler::onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) {
    std::cout << "EnemyCollisionHandler "  <<std::endl;
    if (begin){
        auto crystal = other->getGameObject()->getComponent<CrystalHealth>();
        std::cout << "crystal " << crystal <<std::endl;
        if(crystal) {
            std::cout << "hit crystal "<<std::endl;
            // TODO: move/link this class to EnemyComponent? So it can be read from a json file with other stats!
            crystal->decreaseHealth(25);
        }
    }
}

void EnemyCollisionHandler::onCollisionEnd(size_t collisionId) {
    std::cout << "Collision end "<<collisionId<<std::endl;
}