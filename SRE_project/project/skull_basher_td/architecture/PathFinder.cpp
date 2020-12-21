
#include "PathFinder.hpp"
#include "Transform.hpp"
#include "Scene.hpp"
#include <cmath>

PathFinder::PathFinder(GameObject* gameObject)
 : Component(gameObject)
{
    PathFinder::pfMoveSpeed = 0.1f;
    // auto currentScene = gameObject->getScene();
    // currentPathIndex = currentScene->gameManager->getFirstPathIndex();
    currentPathIndex = gameObject->getScene()->gameManager->getFirstPathIndex();
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    fetchNextPathPoint();
}

PathFinder::~PathFinder()
{

}

void PathFinder::fetchNextPathPoint(){
    //get the scene
    auto currentScene = gameObject->getScene()->gameManager;
    
    nextPathPoint = currentScene->getNextPathPoint(currentPathIndex);
    // auto thing = currentScene->gameManager->getNextPathPoint(currentPathIndex);

    //get the next path point
    // auto currentGameObject = this->getGameObject();
    // auto thing = this->gameObject->getName();
    // nextPathPoint = this->getGameObject()->getScene()->gameManager->getNextPathPoint(currentPathIndex);

    //update current path index
    if (currentPathIndex > 0)
    {
        currentPathIndex -= 1;
    } else if (currentPathIndex == 0)
    {

    }
    
}

void PathFinder::update(float deltaTime){
    std::cout << "delta time is: " << deltaTime << "\n";
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    //get current position
    auto currentTransform = gameObject->getComponent<Transform>();

    currentPosition = currentTransform->position;
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
    //update transform
    // auto t = glm::smoothstep(0.0f, keyframe->timeDuration, currentAnimation.second->getCurrentKeyframeTime());
 
    // nextPosition.x = glm::mix(nextPathPoint.x, currentPosition.x, 0.01f);
    // // nextPosition.y = glm::mix(nextPathPoint.y, currentPosition.y, 0.01f);
    // nextPosition.z = glm::mix(nextPathPoint.z, currentPosition.z, 0.01f);
    nextPosition.x = glm::mix(currentPosition.x, nextPathPoint.x, pfMoveSpeed); //speed is controlled with the float
    // nextPosition.y = glm::mix(currentPosition.y, nextPathPoint.y, 0.01f);
    nextPosition.z = glm::mix(currentPosition.z, nextPathPoint.z, pfMoveSpeed);
    // nextPosition = glm::mix(currentPosition, nextPathPoint, 0.01f);

    // nextPosition.x = glm::smoothstep(nextPathPoint.x, currentPosition.x, 0.1f);
    // nextPosition.y = glm::smoothstep(nextPathPoint.y, currentPosition.y, 0.1f);
    // nextPosition.z = glm::smoothstep(nextPathPoint.z, currentPosition.z, 0.1f);
    // nextPosition.x = glm::smoothstep(currentPosition.x, nextPathPoint.x, 0.1f);
    // nextPosition.y = glm::smoothstep(currentPosition.y, nextPathPoint.y, 0.1f);
    // nextPosition.z = glm::smoothstep(currentPosition.z, nextPathPoint.z, 0.1f);

    // nextPosition = glm::mix(currentPosition, nextPathPoint, velocity);
    nextPosition.y = 0; // correction for the path being on the floor
    std::cout << "I am a skull, I should be moving to: " << nextPosition.x << "\n";

    currentTransform->position = nextPosition;
}