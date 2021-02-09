
#include "Pathfinder.hpp"
#include "Transform.hpp"

#include "GameObject.hpp"
#include "./physics/RigidBody.hpp"

// sound includes
#include "./sound/SourceManager.hpp"
#include "./sound/PlaylistComponent.hpp"

using namespace glm;
Pathfinder::Pathfinder(GameObject* _gameObject)
: gameObject(_gameObject) {

    // initialise pathfinder
    currentPathIndex = GameManager::getInstance().getFirstPathIndex(); // get the first index (can change depending on how long the path is)
    fetchNextPathPoint();
    currentPosition = gameObject->getComponent<Transform>()->position;
    startPathPoint = currentPosition;
    direction = glm::normalize(nextPathPoint-startPathPoint);
}

void Pathfinder::fetchNextPathPoint(){

    if (currentPathIndex > 0)
        currentPathIndex -= 1;
    nextPathPoint = GameManager::getInstance().getPathPoint(currentPathIndex);
}

void Pathfinder::update(float deltaTime) {
    if (moving) {
        //only if the skull has been set to moving it should move along the path
        
        btRigidBody* rigidBody = nullptr; // init a rigid body

        auto transformComp = gameObject->getComponent<Transform>();
        auto rigidBodyComp = gameObject->getComponent<RigidBody>();

        if(rigidBodyComp)
            rigidBody = rigidBodyComp->getRigidBody(); // if skull has rigid body, get it

        if (rigidBody) {
            // skull has rigid body, get the current position of it
            btTransform currentTransform = rigidBody->getWorldTransform();
            auto & origin = currentTransform.getOrigin();
            currentPosition = {origin.x(), origin.y(), origin.z()};
        } else if (transformComp)
            currentPosition = transformComp->position; // no rigid body, use Transform instead

        auto error = 0.5f;
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


        // mix currentposition with next path point and delta time
        // TODO: review
        nextPosition = currentPosition + moveSpeed * direction * deltaTime;
        nextPosition.y = 0; // make sure it's on ground

        // update the world transform of the skull (make it move)
        if (rigidBody) {
            // convert Transform vriables to bullet variables
            btVector3 nextBtPosition = {nextPosition.x, nextPosition.y, nextPosition.z};
            btTransform transform = rigidBody->getWorldTransform();
            transform.setOrigin(nextBtPosition);

            if (transformComp) {
                // Set orientation
                float newRotation = transformComp->rotation.y;
                btQuaternion aroundX;
                aroundX.setRotation(btVector3(0,-1,0), radians(newRotation)); // works, but the objects face the wrong way?
                transform.setRotation(aroundX);
                rigidBody->setCenterOfMassTransform(transform);
            }
            rigidBody->getMotionState()->setWorldTransform(transform); 
            rigidBody->activate(true); // activate skull (to make sure it collides with things)
        }
    }
}

void Pathfinder::setMoving(bool incomingMovingStatus){
    // change moving status and do special things on this event

    moving = incomingMovingStatus;
    if (moving == true)
    {
        // TODO check if sound can be moved to a memeber variable for better performance
        std::string moveSound;
        gameObject->getComponent<PlaylistComponent>()->getSoundEffectName("spawn", moveSound);
        /* play sound */
        SourceManager * mySourceManager = SourceManager::Get();
        mySourceManager->playMyJam(moveSound, currentPosition, 40.f);
    }

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

const vec3 &Pathfinder::getDirection() const {
    return direction;
}

glm::vec3 Pathfinder::previewPathPoint(int pathIndex) {
    return GameManager::getInstance().getPathPoint(pathIndex);
}
