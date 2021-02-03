#pragma once

#include "CollisionHandler.hpp"
#include "../Component.hpp"

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBody;

class EnemyCollisionHandler : public Component, public CollisionHandler {
public:
    // explicit CustomCollisionHandler(GameObject *gameObject) : Component(gameObject));
    explicit EnemyCollisionHandler(GameObject *gameObject);
    virtual ~EnemyCollisionHandler();

    // void onCollision(size_t collisionId, RigidBody *other, glm::vec3 position, bool begin) override {
    //     if (begin){
    //         std::cout << "Collision "<< collisionId <<" on "<<other->getGameObject()->getName()<< " at "<<glm::to_string(position)<<std::endl;
    //     }
    // }
    // void onCollisionEnd(size_t collisionId) override {
    //     std::cout << "Collision end "<<collisionId<<std::endl;
    // }
    // void onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) override;
    void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) override;
    // void onCollision(size_t collisionId, RigidBody *other, glm::vec3 position, bool begin);
    // void onCollisionEnd(size_t collisionId); 
    void onCollisionEnd(size_t collisionId) override; 

};