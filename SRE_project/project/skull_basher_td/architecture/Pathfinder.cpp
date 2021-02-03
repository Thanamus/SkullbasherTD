
#include "Pathfinder.hpp"
#include "Transform.hpp"
#include "scenes/Scene.hpp"
#include <cmath>

#include "./physics/RigidBody.hpp"
#include "./sound/SourceManager.hpp"
#include "../GameManager.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace glm;
Pathfinder::Pathfinder(GameObject* _gameObject)
: gameObject(_gameObject) {
    // Pathfinder::pfMoveSpeed = 0.1f; //not sure if the declaration in the .hpp file is working
    gameManagerInstance = GameManager::getInstance();
    currentPathIndex = GameManager::getInstance().getFirstPathIndex();
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    fetchNextPathPoint();
    currentPosition = gameObject->getComponent<Transform>()->position;
}

void Pathfinder::fetchNextPathPoint(){
    nextPathPoint = GameManager::getInstance().getNextPathPoint(currentPathIndex);

    //update current path index
    if (currentPathIndex > 0)
    {
        currentPathIndex -= 1;
    } else if (currentPathIndex == 0)
    {

    }
    
}

void Pathfinder::update(float deltaTime) {
    // std::cout << "delta time is: " << deltaTime << "\n";
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    if (moving) {
        //only if the skull has been set to moving it should move
        //get current position
//        bool rigidBodyCheck = false;
//        btRigidBody* rigidBody = nullptr;

        auto transformComp = gameObject->getComponent<Transform>();

//        if(rigidBodyComp)
//            rigidBody = rigidBodyComp->getRigidBody();
        // TODO: this bit looks useless since the rigidBody always updates the Transform on each cycle. just get the transform
//        if (rigidBody) {
//            rigidBodyCheck = true;
//            // std::cout << "object has rigid body \n";
//            btTransform currentTransform = rigidBody->getWorldTransform();
//            auto & origin = currentTransform.getOrigin();
//            currentPosition = {origin.x(), origin.y(), origin.z()};
//        } else
        if (transformComp)
            currentPosition = transformComp->position;
        // std::cout << "I am a skull, my current position is: " << currentPosition.x << "\n";

        // get nextposition
        // if (currentPosition.x == nextPathPoint.x && currentPosition.z == nextPathPoint.z)
        if ((currentPosition.x <= nextPathPoint.x+0.5f && currentPosition.x >= nextPathPoint.x-0.5f)  && (currentPosition.z <= nextPathPoint.z+0.5f && currentPosition.z >= nextPathPoint.z-0.5f))
        {
            fetchNextPathPoint();
        }

        // std::cout << "I am a skull, I should be moving to: " << nextPathPoint.x << "\n";
        // move skull
        // mix currentposition with next path point and delta time
        // TODO: review
        nextPosition.x = glm::mix(currentPosition.x, nextPathPoint.x, pfMoveSpeed*deltaTime); //speed is controlled with the float
        // nextPosition.y = glm::mix(currentPosition.y, nextPathPoint.y, 0.01f);
        nextPosition.z = glm::mix(currentPosition.z, nextPathPoint.z, pfMoveSpeed*deltaTime);

        // nextPosition = glm::mix(currentPosition, nextPathPoint, velocity);
        nextPosition.y = 0; // correction for the path being on the floor



        // std::cout << "I am a skull, I should be moving to: " << nextPosition.x << "\n";
        // gameObject->getComponent<Transform>()->lookAt(nextPosition, glm::vec3(0, 1, 0));
        // getGameObject()->getComponent<Transform>()->lookAt(nextPosition, glm::vec3(0, 1, 0));

        if(transformComp)
            transformComp->position = nextPosition;
        //update transform
//        if (rigidBodyCheck)
//        {
//            btVector3 nextBtPosition = {nextPosition.x, nextPosition.y, nextPosition.z};
//            btTransform transform = rigidBody->getWorldTransform();
//            transform.setOrigin(nextBtPosition);
//            rigidBody->getMotionState()->setWorldTransform(transform); // it works!!!!
//            // std::cout << "nest position should be: " << nextBtPosition.x() << std::endl;
//
//            if (hasTransform)
//            {
//                btVector3 nextBtPosition = {nextPosition.x, nextPosition.y, nextPosition.z};
//                btTransform transform = hasRigidBody->getWorldTransform();
//                transform.setOrigin(nextBtPosition);
//
//                // Set orientation
//                float newRotation = getGameObject()->getComponent<Transform>()->rotation.y;
//                btQuaternion aroundX;
//                aroundX.setRotation(btVector3(0,-1,0), radians(newRotation-180)); // works, but the objects face the wrong way?
//                transform.setRotation(aroundX);
//                hasRigidBody->getMotionState()->setWorldTransform(transform); // it works!!!!
//
//                // hasRigidBody->applyCentralImpulse(btVector3{0.01, 0.01, 0.01});
//                // hasRigidBody->setWorldTransform(transform); // it works!!!!
//                hasRigidBody->setCenterOfMassTransform(transform);
//                // btScalar totalForce = hasRigidBody->getLinearVelocity().length();
//                // std::cout << "force on skull is: " << totalForce << std::endl;
//                // std::cout << "nest position should be: " << nextBtPosition.x() << std::endl;
//                /* code */
//            }
//            hasRigidBody->activate(true);
//            // hasRigidBody->setActivationState(DISABLE_DEACTIVATION);
//        }
        
        /* I think position gets updated from RigidBody already*/
        // gameObject->getComponent<Transform>()->position = nextPosition; 

        //currentTransform->position = nextPosition;
        
    }
    


}

void Pathfinder::setMoving(bool incomingMovingStatus){
    moving = incomingMovingStatus;
    if (moving == true)
    {
        /* play sound */
        SourceManager * mySourceManager = SourceManager::Get(); // apparently worked!
        mySourceManager->playMyJam("pestilence.wav", currentPosition, 40.f);
    }

    //play sound
}

bool Pathfinder::isMoving() const{
    return moving;
}

int Pathfinder::getCurrentPathIndex() const {
    return currentPathIndex;
}

float Pathfinder::getMoveSpeed() const {
    return moveSpeed;
}

void Pathfinder::setMoveSpeed(float moveSpeed) {
    Pathfinder::moveSpeed = moveSpeed;
}

//    void Pathfinder::setMoveSpeed(float incoming_move_speed){
//        pfMoveSpeed = incoming_move_speed;
//    }
//
//    float Pathfinder::getMoveSpeed(){
//        return pfMoveSpeed;
//    }