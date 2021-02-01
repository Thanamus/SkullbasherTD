
#include "Pathfinder.hpp"
#include "Transform.hpp"
#include "RigidBody.hpp"
#include "SourceManager.hpp"

using namespace glm;
Pathfinder::Pathfinder(GameObject* _gameObject)
: gameObject(_gameObject) {
    Pathfinder::pfMoveSpeed = 0.1f; //not sure if the declaration in the .hpp file is working

    gameManager = gameObject->getScene()->gameManager;
    currentPathIndex = gameManager->getFirstPathIndex();
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    fetchNextPathPoint();
    currentPosition = gameObject->getComponent<Transform>()->position;
}

void Pathfinder::fetchNextPathPoint(){
    nextPathPoint = gameManager->getNextPathPoint(currentPathIndex);

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
        bool rigidBodyCheck = false;
        btRigidBody* rigidBody = nullptr;

        rigidBody = gameObject->getComponent<RigidBody>()->getRigidBody();
        if (rigidBody) {
            rigidBodyCheck = true;
            // std::cout << "object has rigid body \n";
            btTransform currentTransform = rigidBody->getWorldTransform();
            auto & origin = currentTransform.getOrigin();
            currentPosition = {origin.x(), origin.y(), origin.z()};
            
        } else if(auto currentTransform = gameObject->getComponent<Transform>()) {
            currentPosition = currentTransform->position;
        }

        // std::cout << "I am a skull, my current position is: " << currentPosition.x << "\n";

        //get nextposition
        // if (currentPosition.x == nextPathPoint.x && currentPosition.z == nextPathPoint.z)
        if ((currentPosition.x <= nextPathPoint.x+0.5f && currentPosition.x >= nextPathPoint.x-0.5f)  && (currentPosition.z <= nextPathPoint.z+0.5f && currentPosition.z >= nextPathPoint.z-0.5f))
        {
            fetchNextPathPoint();
        }

        // std::cout << "I am a skull, I should be moving to: " << nextPathPoint.x << "\n";
        //move skull
        //mix currentposition with next path point and delta time
        nextPosition.x = glm::mix(currentPosition.x, nextPathPoint.x, pfMoveSpeed); //speed is controlled with the float
        // nextPosition.y = glm::mix(currentPosition.y, nextPathPoint.y, 0.01f);
        nextPosition.z = glm::mix(currentPosition.z, nextPathPoint.z, pfMoveSpeed);

        // nextPosition = glm::mix(currentPosition, nextPathPoint, velocity);
        nextPosition.y = 0; // correction for the path being on the floor
        // std::cout << "I am a skull, I should be moving to: " << nextPosition.x << "\n";
        gameObject->getComponent<Transform>()->lookAt(nextPosition, glm::vec3(0, 1, 0));

        //update transform
        if (rigidBodyCheck)
        {
            btVector3 nextBtPosition = {nextPosition.x, nextPosition.y, nextPosition.z};
            btTransform transform = rigidBody->getWorldTransform();
            transform.setOrigin(nextBtPosition);
            rigidBody->getMotionState()->setWorldTransform(transform); // it works!!!!
            // std::cout << "nest position should be: " << nextBtPosition.x() << std::endl;
        }
        
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
