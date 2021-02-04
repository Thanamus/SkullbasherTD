//
// Created by Morten Nobel Jørgensen on 2018-11-10.
//

#include "GhostObject.hpp"
#include "../GameObject.hpp"
#include "../Transform.hpp"
#include "BulletPhysics.hpp"
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


#include "BulletCollision/CollisionDispatch/btGhostObject.h"


GhostObject::GhostObject(GameObject* gameObject) : Component(gameObject) {
    transform = gameObject->getComponent<Transform>().get();

}

GhostObject::~GhostObject() {
    // if (rigidBody){
    //     delete fallMotionState;
    //     delete shape;
	// 	if (gameObject->getScene()->bulletPhysics){
	// 		gameObject->getScene()->bulletPhysics->world->removeRigidBody(rigidBody);
	// 	}
    //     delete rigidBody;
    // }
}

// btRigidBody* GhostObject::getRigidBody(){
//     return rigidBody;
// }

void GhostObject::updateTransformFromPhysicsWorld(){
    btTransform pTransform;
    // ghostObject->getMotionState()->getWorldTransform(pTransform);
    pTransform = ghostObject->getWorldTransform();
    auto & origin = pTransform.getOrigin();
    transform->position = {origin.x(), origin.y(), origin.z()};
    auto pRot = pTransform.getRotation();
    glm::quat inputQuat(pRot.w(), pRot.x(), pRot.y(), pRot.z());
    transform->rotation = glm::degrees(glm::eulerAngles(inputQuat));
}

void GhostObject::initGhostObjectWithSphere(float radius){
        // ghostObject = new btGhostObject();
        ghostObject = new btPairCachingGhostObject;
		// ghostObject->setCollisionShape(new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.))));
		ghostObject->setCollisionShape(new btBoxShape(btVector3(btScalar(1.),btScalar(1.),btScalar(1.))));
		
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
        // ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
//    if (rigidBody){
//         physicsWorld->removeRigidBody(rigidBody);
//         delete rigidBody;
//     }
//     rigidBody = new btRigidBody(info);
//     rigidBody->setUserPointer(this);

    // physicsWorld->addRigidBody(rigidBody);
    // physicsWorld->addRigidBody(ghostObject);
        
}

btGhostObject* GhostObject::getGhostObject(){
    return ghostObject;
}

// void GhostObject::motorPreTickCallback (btDynamicsWorld *world, btScalar timeStep)
// {
    
//   	for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++)
//  	{
//                 btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(ghostObject->getOverlappingObject(i));
//                // do whatever you want to do with these pairs of colliding objects
//      }
// }
