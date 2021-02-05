#pragma once

#include "CollisionHandler.hpp"
#include "../Component.hpp"

#include <chrono>

// Forward declarations
class Component;
class CollisionHandler;
class GameObject;
class RigidBody;

class PlayerCollisionHandler : public Component, public CollisionHandler {
public:
    // explicit CustomCollisionHandler(GameObject* gameObject) : Component(gameObject));
    explicit PlayerCollisionHandler(GameObject* gameObject);
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
    void onCollisionEnd(size_t collisionId, GameObject *other) override;
private:
        // Footstep sound timing
    // bool& isGrounded;
    std::chrono::steady_clock::time_point start_footstep_lockout;
    int footstep_lockout_millisec = 300;
};