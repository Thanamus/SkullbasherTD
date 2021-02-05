
#include "CustomCollisionHandler.hpp"

#include "../GameObject.hpp"
#include "../physics/RigidBody.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "ArrowCollisionHandler.hpp"
// #include "../health/CrystalHealth.hpp"

// Sound Effects _Include
#include "../sound/SourceManager.hpp"
#include "../Transform.hpp"
#include "../EnemyComponent.hpp"
#include "../health/CrystalHealth.hpp"

class GameObject;

ArrowCollisionHandler::ArrowCollisionHandler(GameObject *gameObject) : Component(gameObject) {

}

void ArrowCollisionHandler::onCollision(size_t collisionId, GameObject* other, glm::vec3 position, bool begin) {
    if(gameObject->deleteMe)
        return;
    
    if (begin) {
        // auto crystal = other->getComponent<CrystalHealth>(); // crystal health check, could change to enemy health check
        // std::cout << "crystal " << crystal <<std::endl;
        // const std::string objectName = other->getName();

        /* TODO ask andrea what is a better coding pattern
         auto soundeffect = SourceManager::Get(); // get sound effect player
        soundeffect->playMyJam("deathd.wav", otherObjectsPosition, 20);

        OR
        SourceManager::Get()->playMyJam("deathd.wav", otherObjectsPosition, 20);
        */
        // auto soundeffect = SourceManager::Get(); // get sound effect player
        auto enemy = other->getComponent<EnemyComponent>();
        auto otherObjectsPosition = other->getComponent<Transform>()->position;
        if (enemy) // check group ID matches that of the enemy!
        {
            // auto enemyPosition = other->getComponent<Transform>()->position;
            std::cout << "bashing skulls for days" << std::endl;
            // soundeffect->playMyJam("deathd.wav", otherObjectsPosition, 20);
            //SourceManager::Get()->playMyJam("deathd.wav", otherObjectsPosition, 20);
            //other->deleteMe = true;
            enemy->decreaseHealth(1);
            auto rigidbody = gameObject->getComponent<RigidBody>();
            if(rigidbody) // prevents further collisions
                rigidbody->getRigidBody()->setActivationState(WANTS_DEACTIVATION);
            gameObject->deleteMe = true;
        } 
        // else if ( group == BUILDINGS){
        //     // TODO noise is a little annoying, need to make either a time out or make the object goto sleep faster
        //     // update, changed some friction values and now it seems better
                // update, removed sound on building hits, not wanted at the moment
        //     soundeffect->playMyJam("metal-small1.wav", otherObjectsPosition, 1);
            
        // }
        
         
        // if(crystal != nullptr)
        // {
        //     std::cout << "hit crystal "<<std::endl;
        //     crystal->decreasHealth(25);

        //     gameObject->deleteMe = true;
            
        // }

    }
}
// void EnemyCollisionHandler::onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) {
//     std::cout << "EnemyCollisionHandler "  <<std::endl;
//     if (begin){
//         auto crystal = other->getGameObject()->getComponent<CrystalHealth>();
//         std::cout << "crystal " << crystal <<std::endl;
//         if(crystal != nullptr)
//         {
//             std::cout << "hit crystal "<<std::endl;
//             crystal->decreasHealth(25);
//         }
//     }
// }

void ArrowCollisionHandler::onCollisionEnd(size_t collisionId) {
//    std::cout << "Collision end "<<collisionId<<std::endl;
}