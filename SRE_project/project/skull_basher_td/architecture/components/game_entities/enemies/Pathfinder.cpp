
#include "Pathfinder.hpp"
#include "../../TransformComponent.hpp"

#include "../../physics/RigidBodyComponent.hpp"

// sound includes
#include "../../../sound/SourceManager.hpp"
#include "../../sound/PlaylistComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
Pathfinder::Pathfinder(GameObject* _gameObject)
: gameObject(_gameObject) {
    // initialize pathfinder
    currentPathIndex = GameManager::getInstance().getFirstPathIndex(); // get the first index (can change depending on how long the path is)
    fetchNextPathPoint(); // get and set next path point
    currentPosition = gameObject->getComponent<TransformComponent>()->position;
    startPathPoint = currentPosition; // set start
    distance = glm::length(glm::vec2(nextPathPoint.x, nextPathPoint.z) - glm::vec2(startPathPoint.x, startPathPoint.z)); // calc current XZ distance
    direction = glm::normalize(nextPathPoint - startPathPoint); // calc current direction
    glm::mat4 lookAtMat = glm::lookAt(startPathPoint, nextPathPoint, {0, 1, 0});
    auto cosYAngle = (float)sqrt(pow(lookAtMat[0][0], 2) + pow(lookAtMat[1][0], 2));
    // calcs starting rotation to face the path
    rotY = atan2(lookAtMat[2][0], cosYAngle);
}

void Pathfinder::fetchNextPathPoint(){
    if (currentPathIndex > 0)
        currentPathIndex -= 1;
    nextPathPoint = GameManager::getInstance().getPathPoint(currentPathIndex);
}

void Pathfinder::update(float deltaTime) {
    if (moving) {
        //only if the skull has been set to moving it should move along the path
        btRigidBody* rigidBody = nullptr; // rigidbody pointer

        // get transform / rigidbody compinents
        auto transformComp = gameObject->getComponent<TransformComponent>();
        auto rigidBodyComp = gameObject->getComponent<RigidBodyComponent>();

        if(rigidBodyComp)
            rigidBody = rigidBodyComp->getRigidBody(); // if skull has rigid body, get it

        if (rigidBody) {
            // skull has rigid body, get the current position of it
            btTransform currentTransform = rigidBody->getWorldTransform();
            auto & origin = currentTransform.getOrigin();
            currentPosition = {origin.x(), origin.y(), origin.z()};
        } else if (transformComp)
            currentPosition = transformComp->position; // no rigid body, use Transform instead

        // determines how close to get to the end point
        auto error = 0.1f;

        // check if movement has gone too far, prevents the skulls heading off into the sunset
        // if the skull is getting farther from the next path point, it has moved too far
        float newDistance = glm::length(glm::vec2(nextPathPoint.x, nextPathPoint.z) - glm::vec2(currentPosition.x, currentPosition.z));
        bool movedPast = newDistance > distance;
        // if close enough
        bool closeToNext = (abs(currentPosition.x - nextPathPoint.x) <= error && abs(currentPosition.z - nextPathPoint.z) <= error);
        if (movedPast || closeToNext) {
            // updates to move to the next path point
            startPathPoint = nextPathPoint;
            fetchNextPathPoint();
            if(currentPathIndex == 0 && startPathPoint == nextPathPoint) { // has finished moving
                moving = false;
                return;
            }
            // slightly corrects cascading position errors, with glm::mix to be slightly smoother
            currentPosition = glm::mix(currentPosition, startPathPoint, 0.5);
            currentPosition.y = 0;
            direction =  glm::normalize(nextPathPoint - startPathPoint);
            distance = glm::length(nextPathPoint - startPathPoint);
            glm::mat4 lookAtMat = glm::lookAt(startPathPoint, nextPathPoint, {0, 1, 0});
            auto cosYAngle = (float)sqrt(pow(lookAtMat[0][0], 2) + pow(lookAtMat[1][0], 2));
            rotY = atan2(lookAtMat[2][0], cosYAngle);
            if(direction.z < 0) // workaround
                rotY += M_PI;
        } else
            distance = newDistance;

        // move forward along the current direction
        nextPosition = currentPosition + moveSpeed * direction * deltaTime;
        nextPosition.y = 0; // make sure it's on ground

        // update the world transform of the skull (make it move)
        if (rigidBody) {
            // convert Transform variables to bullet variables
            btVector3 nextBtPosition = {nextPosition.x, nextPosition.y, nextPosition.z};
            btTransform transform = rigidBody->getWorldTransform();
            transform.setOrigin(nextBtPosition);
            // Set orientation
            if(!glm::isnan(rotY)) {
                btQuaternion aroundY;
                aroundY.setRotation(btVector3(0,1,0), rotY);
                transform.setRotation(aroundY);
            }
            rigidBody->setCenterOfMassTransform(transform);
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
        /*std::string moveSound;
        gameObject->getComponent<PlaylistComponent>()->getSoundEffectName("spawn", moveSound);
         play sound
        SourceManager * mySourceManager = SourceManager::Get();
        mySourceManager->playMyJam(moveSound, currentPosition, 40.f);*/
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
