//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//
#pragma once

#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "BulletDebugDrawSRE.hpp"
#include "sre/RenderPass.hpp"

class Scene;

// class btGhostPairCallback;
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

class BulletPhysics {
public:
    BulletPhysics();
    ~BulletPhysics();

    const glm::vec3 &getGravity() const;
    void setGravity(const glm::vec3 &gravity);

    void step(Scene* scene);

    void debugDrawNewFrame();
    void debugDraw(sre::RenderPass& renderPass);
    bool BulletPhysics::RaycastWorld(const glm::vec3 &Start, glm::vec3 &End);
    // void GetCollidingObjectsInsidePairCachingGhostObject(btDiscreteDynamicsWorld* m_dynamicsWorld,btPairCachingGhostObject* m_pairCachingGhostObject,btAlignedObjectArray < btCollisionObject* >& collisionArrayOut);
private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* world;

    // btGhostPairCallback* m_ghostPairCallback = NULL;				// Needed once to enable ghost objects inside Bullet
    // btGhostObject* m_ghostObject = NULL;							// simple aabb ghost object (keeps track of the objects whose aabbs intersect its own collision shape aabb: this is called "broadphase stage collision detection")
    // btPairCachingGhostObject* m_pairCachingGhostObject=NULL;

    glm::vec3 gravity = {0, -9.8f, 0};
    float timeStep = 1/60.0f;
    int maxSubSteps = 10;

    BulletDebugDrawSRE debugDrawObj;

    friend class RigidBody;
    friend class GhostObject;
};
