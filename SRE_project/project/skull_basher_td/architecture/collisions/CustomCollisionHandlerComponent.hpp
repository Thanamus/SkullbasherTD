#pragma once

#include "CollisionHandler.hpp"
#include "../Component.hpp"

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBodyComponent;

class CustomCollisionHandlerComponent : public Component, public CollisionHandler {
public:
    // explicit CustomCollisionHandlerComponent(GameObject* gameObject) : Component(gameObject));
    explicit CustomCollisionHandlerComponent(GameObject* gameObject);
    virtual ~CustomCollisionHandlerComponent();

    // void onCollision(size_t collisionId, RigidBodyComponent *other, glm::vec3 position, bool begin) override {
    //     if (begin){
    //         std::cout << "Collision "<< collisionId <<" on "<<other->getGameObject()->getName()<< " at "<<glm::to_string(position)<<std::endl;
    //     }
    // }
    // void onCollisionEnd(size_t collisionId) override {
    //     std::cout << "Collision end "<<collisionId<<std::endl;
    // }
    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;
    // void onCollision(size_t collisionId, RigidBodyComponent* other, glm::vec3 position, bool begin) override; // original before gameObject switch
    // void onCollision(size_t collisionId, RigidBodyComponent *other, glm::vec3 position, bool begin);
    // void onCollisionEnd(size_t collisionId); 
    void onCollisionEnd(size_t collisionId, GameObject *other) override;

};