
#include "PlayerCollisionHandler.hpp"


#include "../GameObject.hpp"
#include "../Component.hpp"
#include "../Transform.hpp"
#include "../physics/RigidBody.hpp"

#include "../PersonController.hpp"

// sound includes
#include "../sound/SoundDevice.hpp"
#include "../sound/SourceManager.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

class GameObject;
class Component;

PlayerCollisionHandler::PlayerCollisionHandler(GameObject* gameObject) : Component(gameObject) {
    // gameObject->getComponent<Transform>();
    // isGrounded = gameObject->getComponent<PersonController>()->isGrounded; // shold link PersonController and isGrounded
    start_footstep_lockout = std::chrono::steady_clock::now(); // initalise the first footstep lockout
}

PlayerCollisionHandler::~PlayerCollisionHandler(){

}


void PlayerCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 col_position, bool begin){
// void PlayerCollisionHandler::onCollision(size_t collisionId, RigidBody* other, glm::vec3 col_position, bool begin){
    if (begin){
        // std::string otherObjectName = other->getGameObject()->getName();
        std::string otherObjectName = other->getName();
        // auto thing = gameObject->getComponent<Transform>();
        glm::vec3 playerPosition = gameObject->getComponent<Transform>()->position;
        bool& isGrounded = gameObject->getComponent<PersonController>()->isGrounded; // shold link PersonController and isGrounded
        std::cout << "Collision "<< collisionId <<" on "<< otherObjectName << " at "<<glm::to_string(col_position)<<std::endl;
        std::cout << "player position is: " << playerPosition.x << " , " << playerPosition.y << " , " << playerPosition.z << std::endl;
       


        // std::cout << "time_elapsed :" << time_elapsed_milli << std::endl;        

        SourceManager * mySource = SourceManager::Get();
        //----- if colliding with ground, play ground sounds
        if (col_position.y < playerPosition.y-0.2f)
        {
            // collision happened below the player

            // check footstep time
            std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
            int time_elapsed_milli = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_footstep_lockout).count();
            
            isGrounded = true; //player just collided with something on it's feet
            // so it could be considered grounded
            
            // TODO match offset to player colision size 
            if (time_elapsed_milli > footstep_lockout_millisec)
            {
                if (otherObjectName == "GrassBlock01D.obj")
                {
                    mySource->playMyJam_global("gassy-footstep1.wav");
                    // std::cout << "playing grassy footstep" << std::endl;
                } else if (otherObjectName == "Floor01.obj"){
                    mySource->playMyJam_global("stepwood_2.wav");
                } else if (otherObjectName == "PathBlock01D.obj"){
                    
                } else if (otherObjectName == "Bridge01D.obj"){
                    
                }

                // restart footstep lockout
                start_footstep_lockout = std::chrono::steady_clock::now();
            }
           
        }
        
        
        
    }
}


void PlayerCollisionHandler::onCollisionEnd(size_t collisionId) {
    std::cout << "Collision end "<<collisionId<<std::endl;
}

