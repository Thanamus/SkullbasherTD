
#include "CustomCollisionHandlerComponent.hpp"


#include "../GameObject.hpp"
#include "../physics/RigidBodyComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

class GameObject;

CustomCollisionHandlerComponent::CustomCollisionHandlerComponent(GameObject* gameObject) : Component(gameObject) {

}

CustomCollisionHandlerComponent::~CustomCollisionHandlerComponent(){

}


void CustomCollisionHandlerComponent::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
//    if (begin){
//        std::cout << "Collision "<< collisionId <<" on "<<other->getName()<< " at "<<glm::to_string(position)<<std::endl;
//    }
}
// Original
// void CustomCollisionHandlerComponent::onCollision(size_t collisionId, RigidBodyComponent* other, glm::vec3 position, bool begin) {
//     if (begin){
//         std::cout << "Collision "<< collisionId <<" on "<<other->getGameObject()->getName()<< " at "<<glm::to_string(position)<<std::endl;
//     }
// }

void CustomCollisionHandlerComponent::onCollisionEnd(size_t collisionId, GameObject *other) {
//    std::cout << "Collision end "<<collisionId<<std::endl;
}

