//
// Created by Morten Nobel Jørgensen on 2018-11-10.
//

#include "RigidBodyComponent.hpp"
#include "../GameObject.hpp"
#include "../TransformComponent.hpp"
#include "BulletPhysics.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <cmath>

#include "BulletCollision/CollisionDispatch/btGhostObject.h"


RigidBodyComponent::RigidBodyComponent(GameObject* gameObject) : Component(gameObject) {
    transform = gameObject->getComponent<TransformComponent>().get(); // sets up the new rigid body to use the game objects Transform
}

// remove the (bulet) rigid body from the dynamics world when deleting the component
RigidBodyComponent::~RigidBodyComponent() {
    transform = nullptr;
    if (rigidBody) {
        delete fallMotionState;
        delete shape;
        auto physics = gameObject->getScene()->bulletPhysics;
		if (physics)
			physics->world->removeRigidBody(rigidBody);
        delete rigidBody;
    }
}

btRigidBody* RigidBodyComponent::getRigidBody(){
    return rigidBody; // returns the (bullet) rigid body
}

void RigidBodyComponent::updateTransformFromPhysicsWorld(){
        btTransform pTransform;
        if(rigidBody) {
            pTransform  = rigidBody->getWorldTransform();
            auto & origin = pTransform.getOrigin();
            if(transform) {
                // update taking into account how child transforms work
//                glm::vec3 prevGlobalPos = transform->globalPosition();
//                glm::vec3 newGlobalPos = {origin.x(), origin.y(), origin.z()};
//                transform->position += (newGlobalPos - prevGlobalPos);
                transform->position = { origin.x(), origin.y(), origin.z() };
                auto pRot = pTransform.getRotation();
                glm::quat inputQuat(pRot.w(), pRot.x(), pRot.y(), pRot.z());
                transform->rotation = glm::degrees(glm::eulerAngles(inputQuat));
            }
        }
}

// adds (bullet) rigid body to the physics world - common to all shapes
// version that also adds masks and group fileters
void RigidBodyComponent::initRigidBody(const btRigidBody::btRigidBodyConstructionInfo& info, short group, short mask){
    auto physicsWorld = gameObject->getScene()->bulletPhysics->world; // get the physics world
   
    if (rigidBody){ // make sure that there is only one rigid body on the game object
        physicsWorld->removeRigidBody(rigidBody);
        delete rigidBody;
    }
    rigidBody = new btRigidBody(info);
    rigidBody->setUserPointer(this); // user pointer is the 'game object' - for use in collision handling later
    const int CF_CUSTOM_MATERIAL_CALLBACK = 8;

//    // --- new stuff for trying to implement kinematic physics
//    bool isDynamic = false;
//    if (info.m_mass != 0)
//    {
//        isDynamic = true;
//        // rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | CF_CUSTOM_MATERIAL_CALLBACK); // original Call
//    } else {
//        // std::cout << "trying to make Kinematic" << std::endl;
//        // rigidBody->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
//        // rigidBody->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
//        // rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
//        // rigidBody->forceActivationState(DISABLE_DEACTIVATION);
//    }

     // physicsWorld->addCollisionObject(rigidBody) // old way

    // set group to group of objects this thing (the object) belongs to
    // mask to the bitwise (PLAYER | BUILDING | WALL)
    // constructor needs to take in which group and object to collide with
    // int group = 2;

    // eg if a player
    // int group = PLAYER;
    // int mask = BUILDINGS | ENEMIES | COINS ;

    // eg if a enemy
    // int group = ENEMY;
    // int mask = PLAYER | CRYSTAL | PROJECTILES ;

    physicsWorld->addRigidBody(rigidBody, group, mask);
}

// adds (bullet) rigid body to the physics world - common to all shapes
// version without mask and group filtering
void RigidBodyComponent::initRigidBody(const btRigidBody::btRigidBodyConstructionInfo& info){
    auto physicsWorld = gameObject->getScene()->bulletPhysics->world;
    if (rigidBody){
        physicsWorld->removeRigidBody(rigidBody);
        delete rigidBody;
    }
    rigidBody = new btRigidBody(info);
    rigidBody->setUserPointer(this);
    const int CF_CUSTOM_MATERIAL_CALLBACK = 8;

    // --- new stuff for trying to implement kinematic physics
    bool isDynamic = false;
//    if (info.m_mass != 0)
//    {
//        isDynamic = true;
//        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | CF_CUSTOM_MATERIAL_CALLBACK); // original Call
//    } else {
//        // std::cout << "trying to make Kinematic" << std::endl;
//        rigidBody->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
//        // rigidBody->forceActivationState(DISABLE_DEACTIVATION);
//    }

    // ---------- end trying to add kinematic physics
    physicsWorld->addRigidBody(rigidBody);
}

// init a sphere shape collision object (no custom filters)
void RigidBodyComponent::initRigidBodyWithSphere(float radius, float mass) {
    delete fallMotionState;
    delete shape;
    shape = new btSphereShape(radius);
    shape->setMargin(0.004);
    auto pos = transform->globalPosition();
    auto rot = transform->localRotation();
    glm::quat rotQ = glm::quat_cast(rot);

    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z)));

//-----------
    bool isDynamic = false;
    if (mass != 0)
    {
        isDynamic = true;
    }
//----------
    if (isDynamic)
    {
        /* code */
        btVector3 fallInertia(0, 0, 0);
        shape->calculateLocalInertia(mass, fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape, fallInertia);
        initRigidBody(fallRigidBodyCI);
    } else {
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape);
        initRigidBody(fallRigidBodyCI);

    }

}

// -------- New sphere init that takes in group and mask values
void RigidBodyComponent::initRigidBodyWithSphere(float radius, float mass, short group, short mask) {
    this->group = group;
    this->mask = mask;

    delete fallMotionState;
    delete shape;
    shape = new btSphereShape(radius);
    shape->setMargin(0.004);

    auto pos = transform->globalPosition();
    auto rot = transform->localRotation();
    glm::quat rotQ = glm::quat_cast(rot);

    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z)));

//-----------
    bool isDynamic = false;
    if (mass != 0)
    {
        isDynamic = true;
    }
//----------
    if (isDynamic)
    {
        /* code */
        btVector3 fallInertia(0, 0, 0);
        shape->calculateLocalInertia(mass, fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape, fallInertia);
        initRigidBody(fallRigidBodyCI, group, mask);
    } else {
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape);
        initRigidBody(fallRigidBodyCI, group, mask);

    }

    // set group to group of objects this thing (the object) belongs to
    // mask to the bitwise (PLAYER | BUILDING | WALL)
    // constructor needs to take in which group and object to collide with
    // int group = 2;

    // eg if a player
    // int group = PLAYER;
    // int mask = BUILDINGS | ENEMIES | COINS ;

    // eg if a enemy
    // int group = ENEMY;
    // int mask = PLAYER | CRYSTAL | PROJECTILES ;

}

// init a box shape collision object (no custom filters)
void RigidBodyComponent::initRigidBodyWithBox(glm::vec3 halfExtend, float mass) {
    delete fallMotionState;
    delete shape;
    shape = new btBoxShape({halfExtend.x, halfExtend.y, halfExtend.z});
    shape->setMargin(0.004);
    auto pos = transform->globalPosition();;
    auto rot = transform->localRotation();
    glm::quat rotQ = glm::quat_cast(rot);

    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z)));

    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape, fallInertia);
    initRigidBody(fallRigidBodyCI);

}

// new initialiser that uses groups and masks
void RigidBodyComponent::initRigidBodyWithBox(glm::vec3 halfExtend, float mass, short group, short mask) {
    this->group = group;
    this->mask = mask;
    
    delete fallMotionState;
    delete shape;
    shape = new btBoxShape({halfExtend.x, halfExtend.y, halfExtend.z});
    shape->setMargin(0.004);
    auto pos = transform->globalPosition();
    auto rot = transform->localRotation();
    glm::quat rotQ = glm::quat_cast(rot);

    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z)));

    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape, fallInertia);
    initRigidBody(fallRigidBodyCI, group, mask);
    // set group to group of objects this thing (the object) belongs to
    // mask to the bitwise (PLAYER | BUILDING | WALL)
    // constructor needs to take in which group and object to collide with
    // int group = 2;

    // eg if a player
    // int group = PLAYER;
    // int mask = BUILDINGS | ENEMIES | COINS ;

    // eg if a enemy
    // int group = ENEMY;
    // int mask = PLAYER | CRYSTAL | PROJECTILES ;

}

// init a plane shape collision object (no custom filters)
void RigidBodyComponent::initRigidBodyWithStaticPlane(glm::vec3 planeNormal, float planeDist) {
    delete fallMotionState;
    delete shape;
    shape = new btStaticPlaneShape({planeNormal.x, planeNormal.y, planeNormal.z}, planeDist);
    auto pos = transform->globalPosition();;
    auto rot = transform->localRotation();
    glm::quat rotQ = glm::quat_cast(rot);

    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z)));

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0, fallMotionState, shape);
    initRigidBody(fallRigidBodyCI);
}


// additions
// can probably remove this
void RigidBodyComponent::setLinearVelocityOnRigidBody(btVector3 linear_velocity){
    rigidBody->setLinearVelocity(linear_velocity);
    btVector3 test = rigidBody->getLinearVelocity();

    // btScalar &thing;
    // test.y(thing);
    std::cout << "linear velocity set, is: " << test.y() << " should be " << linear_velocity.y() << std::endl;
}

short RigidBodyComponent::getGroupID() const{
    return group;
}