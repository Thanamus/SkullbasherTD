
#include "CustomCollisionHandler.hpp"


#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

class GameObject;

CustomCollisionHandler::CustomCollisionHandler(GameObject* gameObject) : Component(gameObject) {

}

CustomCollisionHandler::~CustomCollisionHandler(){

}


void CustomCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
//    if (begin){
//        std::cout << "Collision "<< collisionId <<" on "<<other->getName()<< " at "<<glm::to_string(position)<<std::endl;
//    }
}
// Original
// void CustomCollisionHandler::onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) {
//     if (begin){
//         std::cout << "Collision "<< collisionId <<" on "<<other->getGameObject()->getName()<< " at "<<glm::to_string(position)<<std::endl;
//     }
// }

void CustomCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {
//    std::cout << "Collision end "<<collisionId<<std::endl;
}

