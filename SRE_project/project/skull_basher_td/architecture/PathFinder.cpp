
#include "PathFinder.hpp"
#include "Transform.hpp"
#include "Scene.hpp"
#include <cmath>

PathFinder::PathFinder(GameObject* gameObject)
 : Component(gameObject)
{
    PathFinder::pfMoveSpeed = 0.1f; //not sure if the declaration in the .hpp file is working

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
    nextPosition.x = glm::mix(currentPosition.x, nextPathPoint.x, pfMoveSpeed); //speed is controlled with the float
    // nextPosition.y = glm::mix(currentPosition.y, nextPathPoint.y, 0.01f);
    nextPosition.z = glm::mix(currentPosition.z, nextPathPoint.z, pfMoveSpeed);

    // nextPosition = glm::mix(currentPosition, nextPathPoint, velocity);
    nextPosition.y = 0; // correction for the path being on the floor
    // std::cout << "I am a skull, I should be moving to: " << nextPosition.x << "\n";

    //update transform
    currentTransform->position = nextPosition;
}