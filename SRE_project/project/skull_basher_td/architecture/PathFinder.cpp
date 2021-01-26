
#include "PathFinder.hpp"
#include "Transform.hpp"
#include "Scene.hpp"
#include <cmath>

#include "RigidBody.hpp"

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
    // std::cout << "delta time is: " << deltaTime << "\n";
    // std::cout << "I am a skull, my current path index is: " << currentPathIndex << "\n";
    if (moving)
    { //only if the skull has been set to moving should it move
        //get current position
        bool rigidBodyCheck = false;
        btRigidBody* hasRigidBody = nullptr;

        if (hasRigidBody = gameObject->getComponent<RigidBody>()->getRigidBody())
        {
            rigidBodyCheck = true;
            // std::cout << "object has rigid body \n";
            btTransform currentTransform = hasRigidBody->getWorldTransform();
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

        //update transform
        if (rigidBodyCheck)
        {
            btVector3 nextBtPosition = {nextPosition.x, nextPosition.y, nextPosition.z};
            btTransform transform = hasRigidBody->getWorldTransform();
            transform.setOrigin(nextBtPosition);
            hasRigidBody->getMotionState()->setWorldTransform(transform); // it works!!!!
            // std::cout << "nest position should be: " << nextBtPosition.x() << std::endl;
        }
        
        /* I think position gets updated from RigidBody already*/
        // gameObject->getComponent<Transform>()->position = nextPosition; 

        //currentTransform->position = nextPosition;
        
    }
    


}


    void PathFinder::setWave(int incomingWaveNumber){
        waveNumber = incomingWaveNumber;
    }

    int PathFinder::getWave(){
        return waveNumber;
    }

    void PathFinder::setEnemyNumber(int incomingEnemyNumber){
        enemyNumber = incomingEnemyNumber;
    }

    int PathFinder::getEnemyNumber(){
        return enemyNumber;
    }

    void PathFinder::setMovingStatus(bool incomingMovingStatus){
        moving = incomingMovingStatus;
    }

    bool PathFinder::getMovingStatus(){
        return moving;
    }

    void PathFinder::setEnemySetNumber(int incomingEnemySet){
        enemySetNumber = incomingEnemySet;
    }

    int PathFinder::getEnemySetNumber() {
        return enemySetNumber;
    }