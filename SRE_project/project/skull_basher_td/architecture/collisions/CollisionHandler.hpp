//
// Created by Morten Nobel Jørgensen on 2018-11-15.
//

#pragma once

#include "glm/glm.hpp"

class RigidBody;
class GameObject;

class CollisionHandler {
public:
    // virtual void onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) = 0; // begin or update
    virtual void onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) = 0; // begin or update
    virtual void onCollisionEnd(size_t collisionId, GameObject* other) = 0;
protected:
    bool stopCollisions = false;
};