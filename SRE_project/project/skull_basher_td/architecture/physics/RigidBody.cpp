//
// Created by Morten Nobel Jørgensen on 2018-11-10.
//

#include "RigidBody.hpp"
#include "../GameObject.hpp"
#include "../Transform.hpp"
#include "BulletPhysics.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


#include "BulletCollision/CollisionDispatch/btGhostObject.h"


RigidBody::RigidBody(GameObject *gameObject) : Component(gameObject) {
    transform = gameObject->getComponent<Transform>().get();

}

RigidBody::~RigidBody() {
    if (rigidBody){
        delete fallMotionState;
        delete shape;
		if (gameObject->getScene()->bulletPhysics){
			gameObject->getScene()->bulletPhysics->world->removeRigidBody(rigidBody);
		}
        delete rigidBody;
    }
}

btRigidBody* RigidBody::getRigidBody(){
    return rigidBody;
}

void RigidBody::updateTransformFromPhysicsWorld(){
    bool transformCheck = false;

    if(gameObject->getComponent<Transform>() != nullptr) transformCheck = true;

    if( transformCheck){

        btTransform pTransform;
        rigidBody->getMotionState()->getWorldTransform(pTransform);
        auto & origin = pTransform.getOrigin();
        transform->position = {origin.x(), origin.y(), origin.z()};
        auto pRot = pTransform.getRotation();
        glm::quat inputQuat(pRot.w(), pRot.x(), pRot.y(), pRot.z());
        transform->rotation = glm::degrees(glm::eulerAngles(inputQuat));
    }
}

void RigidBody::initRigidBody(btRigidBody::btRigidBodyConstructionInfo info){
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
    if (info.m_mass != 0)
    {
        isDynamic = true;
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | CF_CUSTOM_MATERIAL_CALLBACK); // original Call
    } else {
        // std::cout << "trying to make Kinematic" << std::endl;
        // rigidBody->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
        rigidBody->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);
        // rigidBody->forceActivationState(DISABLE_DEACTIVATION);
    }

    // ---------- end

    physicsWorld->addRigidBody(rigidBody);
}

void RigidBody::initRigidBodyWithSphere(float radius, float mass) {
    delete fallMotionState;
    delete shape;
    shape = new btSphereShape(radius);
    auto pos = transform->position;
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

void RigidBody::initGhostObjectWithSphere(float radius){
    auto ghostObject = new btGhostObject();
		ghostObject->setCollisionShape(new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.))));
		
        // adding transform for ghost object
        auto pos = transform->position;
        auto rot = transform->localRotation();
        glm::quat rotQ = glm::quat_cast(rot);

        btTransform initialTransform =
            btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z));
        ghostObject->setWorldTransform(initialTransform);
        auto physicsWorld = gameObject->getScene()->bulletPhysics->world;
		// m_dynamicsWorld->addCollisionObject(ghostObject);
		physicsWorld->addCollisionObject(ghostObject);
		physicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());	

//    if (rigidBody){
//         physicsWorld->removeRigidBody(rigidBody);
//         delete rigidBody;
//     }
//     rigidBody = new btRigidBody(info);
//     rigidBody->setUserPointer(this);

    // physicsWorld->addRigidBody(rigidBody);
    // physicsWorld->addRigidBody(ghostObject);
}

void RigidBody::initRigidBodyWithBox(glm::vec3 halfExtend, float mass) {
    delete fallMotionState;
    delete shape;
    shape = new btBoxShape({halfExtend.x, halfExtend.y, halfExtend.z});
    auto pos = transform->position;
    auto rot = transform->localRotation();
    glm::quat rotQ = glm::quat_cast(rot);

    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z)));

    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape, fallInertia);
    initRigidBody(fallRigidBodyCI);

}

void RigidBody::initRigidBodyWithStaticPlane(glm::vec3 planeNormal, float planeDist) {
    delete fallMotionState;
    delete shape;
    shape = new btStaticPlaneShape({planeNormal.x, planeNormal.y, planeNormal.z}, planeDist);
    auto pos = transform->position;
    auto rot = transform->localRotation();
    glm::quat rotQ = glm::quat_cast(rot);

    fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(rotQ.x, rotQ.y, rotQ.z, rotQ.w), btVector3(pos.x, pos.y, pos.z)));

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0, fallMotionState, shape);
    initRigidBody(fallRigidBodyCI);
}


// additions

void RigidBody::setLinearVelocityOnRigidBody(btVector3 linear_velocity){
    rigidBody->setLinearVelocity(linear_velocity);
    btVector3 test = rigidBody->getLinearVelocity();

    // btScalar &thing;
    // test.y(thing);
    std::cout << "linear velocity set, is: " << test.y() << " should be " << linear_velocity.y() << std::endl;
}
