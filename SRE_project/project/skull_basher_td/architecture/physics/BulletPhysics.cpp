//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "BulletPhysics.hpp"
#include "RigidBody.hpp"
#include "GhostObject.hpp"
#include "../GameObject.hpp"
#include "../scenes/Scene.hpp"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

btGhostPairCallback* m_ghostPairCallback = NULL;				// Needed once to enable ghost objects inside Bullet
btGhostObject* m_ghostObject = NULL;							// simple aabb ghost object (keeps track of the objects whose aabbs intersect its own collision shape aabb: this is called "broadphase stage collision detection")
btPairCachingGhostObject* m_pairCachingGhostObject=NULL;
// "Portable" method; prerequisite call: m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);

struct CollisionId {
    size_t collisionId;
    GameObject* bodyA;
    GameObject* bodyB;

    CollisionId(size_t collisionId, GameObject *bodyA, GameObject *bodyB) : collisionId(collisionId), bodyA(bodyA),
                                                                            bodyB(bodyB) {}
};

namespace {
 Scene* scene;

 bool sceneContains(GameObject* ptr){
     for (const auto& go : scene->getGameObjects()){
         if (go.get() == ptr) {
             return true;
         }
     }
     return false;
 }

// based on https://github.com/kripken/ammo.js/issues/82#issuecomment-426429129
bool contactUpdatedCallback(btManifoldPoint& cp,void* body0,void* body1){
    bool collisionBegin = cp.m_userPersistentData == nullptr;
    auto btRigidBody0 = static_cast<btRigidBody *>(body0);
    auto btRigidBody1 = static_cast<btRigidBody *>(body1);
    auto rigidBody0 = static_cast<RigidBody *>(btRigidBody0->getUserPointer());
    auto rigidBody1 = static_cast<RigidBody *>(btRigidBody1->getUserPointer());

    if (collisionBegin){
        static size_t collisionId = 0;
        collisionId++;
//        auto name0 = rigidBody0->getGameObject()->getName();
//        std::cout << "collision on: " << name0 << std::endl;
//        auto name1 = rigidBody1->getGameObject()->getName();
//        std::cout << "collision on: " << name1 << std::endl;

        cp.m_userPersistentData = new CollisionId(collisionId, rigidBody0->getGameObject(), rigidBody1->getGameObject()); //original
    }
    auto* id = (CollisionId*)cp.m_userPersistentData;
    glm::vec3 pointOnA (cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z());
    glm::vec3 pointOnB (cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y(), cp.getPositionWorldOnB().z());

    // for (auto ph : body0GameObject->getCollisionHandlers()){
    for (auto ph : rigidBody0->getGameObject()->getCollisionHandlers()){
        // ph->onCollision(id->collisionId, body1GameObject, pointOnA, collisionBegin); // original
        ph->onCollision(id->collisionId, rigidBody1->getGameObject(), pointOnA, collisionBegin);
    }
    for (auto ph : rigidBody1->getGameObject()->getCollisionHandlers()){
    // for (auto ph : body1GameObject->getCollisionHandlers()){
        ph->onCollision(id->collisionId, rigidBody0->getGameObject(), pointOnB, collisionBegin);
        // ph->onCollision(id->collisionId, body0GameObject, pointOnB, collisionBegin);
    }
    return true;
}

bool contactDestroyedCallback(void * data) {
    auto* id = (CollisionId*)data;
    if (sceneContains(id->bodyA)){
        for (auto ph : id->bodyA->getCollisionHandlers()){
            ph->onCollisionEnd(id->collisionId, id->bodyB);
        }
    }
    if (sceneContains(id->bodyB)){
        for (auto ph : id->bodyB->getCollisionHandlers()){
            ph->onCollisionEnd(id->collisionId, id->bodyA);
        }
    }
    delete id;
    return true;
}

// Performs raycasting on the world and returns the point of collision
}

BulletPhysics::BulletPhysics() {
    gContactProcessedCallback = contactUpdatedCallback;
    gContactDestroyedCallback = contactDestroyedCallback;
    // gContactDestroyedCallback = contactDestroyedCallback;

    // gMotorPreTickCallback = motorPreTickCallback;

    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    world->setDebugDrawer(&debugDrawObj);

    setGravity(gravity);

    // from: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=6195
    m_ghostPairCallback = new btGhostPairCallback(); 
	world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);	
    // based on https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=7468
    // world->setInternalTickCallback(motorPreTickCallback,this,true);

}

BulletPhysics::~BulletPhysics() {
    delete world;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

const glm::vec3 &BulletPhysics::getGravity() const {
    return gravity;
}

void BulletPhysics::setGravity(const glm::vec3 &gravity) {
    BulletPhysics::gravity = gravity;
    world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void BulletPhysics::step(Scene* scene_) {
    scene = scene_;
    world->stepSimulation(timeStep, maxSubSteps);

}

void BulletPhysics::debugDrawNewFrame() {
    debugDrawObj.newFrame();
    world->debugDrawWorld();
}

void BulletPhysics::debugDraw(sre::RenderPass &renderPass) {
    debugDrawObj.render(renderPass);
}

bool BulletPhysics::RaycastWorld(const glm::vec3 &Start, glm::vec3 &End) {

    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(Start.x, Start.y, Start.z), btVector3(End.x, End.y, End.z));
    //RayCallback.m_collisionFilterMask = came;

    // Perform raycast
    world->rayTest(btVector3(Start.x, Start.y, Start.z), btVector3(End.x, End.y, End.z), RayCallback);
    if(RayCallback.hasHit()) {

        btVector3 target = RayCallback.m_hitPointWorld;
        btVector3 Normal = RayCallback.m_hitNormalWorld;
        return true;
    }

    return false;
}
