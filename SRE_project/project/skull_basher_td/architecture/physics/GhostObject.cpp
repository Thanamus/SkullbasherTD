//
// Created by Morten Nobel Jørgensen on 2018-11-10.
//

#include "GhostObject.hpp"
#include "../GameObject.hpp"
#include "../TransformComponent.hpp"
#include "BulletPhysics.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


#include "BulletCollision/CollisionDispatch/btGhostObject.h"

/*
    Ghost object is experimental. It was used as an attempt to get kinematic objects colliding
*/

GhostObject::GhostObject(GameObject* gameObject) : Component(gameObject) {
    transform = gameObject->getComponent<TransformComponent>().get(); // attaches the ghost object to a game object

}

GhostObject::~GhostObject() {

}



void GhostObject::updateTransformFromPhysicsWorld(){
    // if the game object moves in the physics world, it should update the game object's transform
    btTransform pTransform;

    pTransform = ghostObject->getWorldTransform();
    auto & origin = pTransform.getOrigin();
    transform->position = {origin.x(), origin.y(), origin.z()};
    auto pRot = pTransform.getRotation();
    glm::quat inputQuat(pRot.w(), pRot.x(), pRot.y(), pRot.z());
    transform->rotation = glm::degrees(glm::eulerAngles(inputQuat));
}

void GhostObject::initGhostObjectWithSphere(float radius){

        // ghostObject = new btGhostObject(); // not sure if btGhostObject or btPairCachingGhostObject is needed
        ghostObject = new btPairCachingGhostObject;
		// ghostObject->setCollisionShape(new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.))));
		ghostObject->setCollisionShape(new btBoxShape(btVector3(btScalar(1.),btScalar(1.),btScalar(1.)))); // Sphere shape not used. rembmer this is experimental and not used
		
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

        ghostObject->setUserPointer(this);
        ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
        ghostObject->setActivationState(DISABLE_DEACTIVATION);
        
}

btGhostObject* GhostObject::getGhostObject(){
    // return a pointer to the ghost object
    return ghostObject;
}
