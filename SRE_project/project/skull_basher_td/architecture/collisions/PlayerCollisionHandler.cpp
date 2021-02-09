
#include "PlayerCollisionHandler.hpp"


#include "../GameObject.hpp"
#include "../Component.hpp"
#include "../Transform.hpp"

#include "../PersonController.hpp"

// sound includes
#include "../sound/SourceManager.hpp"

#include "../WorldObject.hpp"

class GameObject; // needed for referencing the game object
class Component;

/*
    PlayerCollisionHandler handles collisions that happen on the PersonController (when correctly attached)
*/

PlayerCollisionHandler::PlayerCollisionHandler(GameObject* gameObject) : Component(gameObject) {

    start_footstep_lockout = std::chrono::steady_clock::now(); // initalise the first footstep lockout
}


void PlayerCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 col_position, bool begin){
    if (begin){
//        std::cout << "Collision "<< collisionId <<" on "<< other->getName() << " at "<<glm::to_string(col_position)<<std::endl;


        // check if the other object is a world object
        if(other->getComponent<WorldObject>())
        {

            // other object is a world object, get the objects name
            std::string otherObjectName = other->getName();
            glm::vec3 playerPosition = gameObject->getComponent<Transform>()->position; // TODO check can be removed
            bool& isGrounded = gameObject->getComponent<PersonController>()->isGrounded; // check if grounded
            
            // debugging couts
            // std::cout << "Collision "<< collisionId <<" on "<< otherObjectName << " at "<<glm::to_string(col_position)<<std::endl;
            // std::cout << "player position is: " << playerPosition.x << " , " << playerPosition.y << " , " << playerPosition.z << std::endl;

            // get the SourceManager for playing a sound
            SourceManager * mySource = SourceManager::Get();

            //----- if colliding with ground, play ground sounds
            if (col_position.y < playerPosition.y-0.2f)
            {
                // collision happened below the player (i.e. is likely the ground)

                // check footstep time
                std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
                int time_elapsed_milli = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_footstep_lockout).count();

                isGrounded = true; //player just collided with something on it's feet
                // so it could be considered grounded

                
                if (time_elapsed_milli > footstep_lockout_millisec)
                {
                    if (otherObjectName == "GrassBlock01D.obj")
                    {
                        mySource->playMyJam_global("gassy-footstep1.wav");

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
}


void PlayerCollisionHandler::onCollisionEnd(size_t collisionId, GameObject *other) {
//    std::cout << "Collision end "<<collisionId<<std::endl;
}

