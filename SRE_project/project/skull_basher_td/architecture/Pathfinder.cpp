
#include "Pathfinder.hpp"
#include "Transform.hpp"

#include "./physics/RigidBody.hpp"
#include "./sound/SourceManager.hpp"

using namespace glm;
Pathfinder::Pathfinder(GameObject* _gameObject)
: gameObject(_gameObject) {
    // Pathfinder::moveSpeed = 0.1f; //not sure if the declaration in the .hpp file is working
    currentPathIndex = GameManager::getInstance().getFirstPathIndex();
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    fetchNextPathPoint();
    currentPosition = gameObject->getComponent<Transform>()->position;
    startPathPoint = currentPosition;
    direction = glm::normalize(nextPathPoint-startPathPoint);
}

void Pathfinder::fetchNextPathPoint(){
    nextPathPoint = GameManager::getInstance().getNextPathPoint(currentPathIndex);
    //update current path index
    if (currentPathIndex > 0)
        currentPathIndex -= 1;
}

void Pathfinder::update(float deltaTime) {
    // std::cout << "delta time is: " << deltaTime << "\n";
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    if (moving) {
        //only if the skull has been set to moving it should move
        //get current position
//        bool rigidBodyCheck = false;
        btRigidBody* rigidBody = nullptr;

        auto transformComp = gameObject->getComponent<Transform>();
        auto rigidBodyComp = gameObject->getComponent<RigidBody>();
        if(rigidBodyComp)
            rigidBody = rigidBodyComp->getRigidBody();
        if (rigidBody) {
            btTransform currentTransform = rigidBody->getWorldTransform();
            auto & origin = currentTransform.getOrigin();
            currentPosition = {origin.x(), origin.y(), origin.z()};
        } else if (transformComp)
            currentPosition = transformComp->position;
        // std::cout << "I am a skull, my current position is: " << currentPosition.x << "\n";

        // get nextposition
        // if (currentPosition.x == nextPathPoint.x && currentPosition.z == nextPathPoint.z)
        auto error = 0.3f;
        if (abs(currentPosition.x - nextPathPoint.x) <= error && abs(currentPosition.z - nextPathPoint.z) <= error) {
            startPathPoint = nextPathPoint;
            fetchNextPathPoint();
            if(currentPathIndex == 0 && startPathPoint == nextPathPoint) { // has finished moving
                moving = false;
                return;
            }
            // prevent cascading errors
            currentPosition = startPathPoint;
            currentPosition.y = 0;
            direction =  glm::normalize(nextPathPoint - startPathPoint);
        }
        // std::cout << "I am a skull, I should be moving to: " << nextPathPoint.x << "\n";
        // move skull
        // mix currentposition with next path point and delta time
        // TODO: review
        nextPosition = currentPosition + moveSpeed * direction * deltaTime;
        nextPosition.y = 0; // make sure it's on ground


        // std::cout << "I am a skull, I should be moving to: " << nextPosition.x << "\n";
        // gameObject->getComponent<Transform>()->lookAt(nextPosition, glm::vec3(0, 1, 0));

//
        if(transformComp)
            transformComp->lookAt(nextPosition, glm::vec3(0, 1, 0));
        //update transform
        if (rigidBody) {
            btVector3 nextBtPosition = {nextPosition.x, nextPosition.y, nextPosition.z};
            btTransform transform = rigidBody->getWorldTransform();
            transform.setOrigin(nextBtPosition);
            // std::cout << "nest position should be: " << nextBtPosition.x() << std::endl;

            if (transformComp) {
                // Set orientation
                float newRotation = transformComp->rotation.y;
                btQuaternion aroundX;
                aroundX.setRotation(btVector3(0,-1,0), radians(newRotation)); // works, but the objects face the wrong way?
                transform.setRotation(aroundX);
                // hasRigidBody->applyCentralImpulse(btVector3{0.01, 0.01, 0.01});
                // hasRigidBody->setWorldTransform(transform); // it works!!!!
                rigidBody->setCenterOfMassTransform(transform);
                // btScalar totalForce = hasRigidBody->getLinearVelocity().length();
                // std::cout << "force on skull is: " << totalForce << std::endl;
                // std::cout << "nest position should be: " << nextBtPosition.x() << std::endl;
                /* code */
            }
            rigidBody->getMotionState()->setWorldTransform(transform); // it works!!!!
            rigidBody->activate(true);
            // hasRigidBody->setActivationState(DISABLE_DEACTIVATION);
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

float Pathfinder::getMoveSpeed() const {
    return moveSpeed;
}

void Pathfinder::setMoveSpeed(float moveSpeed_) {
    Pathfinder::moveSpeed = moveSpeed_;
}

const vec3 &Pathfinder::getStartPathPoint() const {
    return startPathPoint;
}

const vec3 &Pathfinder::getNextPathPoint() const {
    return nextPathPoint;
}

//    void Pathfinder::setMoveSpeed(float incoming_move_speed){
//        moveSpeed = incoming_move_speed;
//    }
//
//    float Pathfinder::getMoveSpeed(){
//        return moveSpeed;
//    }