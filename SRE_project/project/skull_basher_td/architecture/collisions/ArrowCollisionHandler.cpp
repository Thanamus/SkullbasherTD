
#include "CustomCollisionHandler.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "ArrowCollisionHandler.hpp"
// #include "../health/CrystalHealth.hpp"

class GameObject;

ArrowCollisionHandler::ArrowCollisionHandler(GameObject *gameObject) : Component(gameObject) {

}

ArrowCollisionHandler::~ArrowCollisionHandler(){

}


void ArrowCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    // std::cout << "EnemyCollisionHandler "  <<std::endl;
    
    if (begin){

        // auto crystal = other->getComponent<CrystalHealth>(); // crystal health check, could change to enemy health check
        // std::cout << "crystal " << crystal <<std::endl;
        // const std::string objectName = other->getName();

        short group = other->getComponent<RigidBody>()->getGroupID();
        if (group == ENEMIES)
        {
            std::cout << "bashing skulls for days" << std::endl;
            other->deleteMe = true;
        }
        
        // if (objectName == "Skull01.obj")
        // {
        //     // very crude collision check: TODO refine collision check
        //     // Suggestion, check for rigid body and group ID?
            
        //     other->deleteMe = true; // set the delete flag on the other object
        // }
        
        // if(crystal != nullptr)
        // {
        //     std::cout << "hit crystal "<<std::endl;
        //     crystal->decreasHealth(25);

        //     gameObject->deleteMe = true;
            
        // }

    }
}
// void EnemyCollisionHandler::onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) {
//     std::cout << "EnemyCollisionHandler "  <<std::endl;
//     if (begin){
//         auto crystal = other->getGameObject()->getComponent<CrystalHealth>();
//         std::cout << "crystal " << crystal <<std::endl;
//         if(crystal != nullptr)
//         {
//             std::cout << "hit crystal "<<std::endl;
//             crystal->decreasHealth(25);
//         }
//     }
// }

void ArrowCollisionHandler::onCollisionEnd(size_t collisionId) {
    std::cout << "Collision end "<<collisionId<<std::endl;
}