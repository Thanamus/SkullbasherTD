//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "BirdController.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "BirdGame.hpp"
#include "SpriteComponent.hpp"
#include <math.h>
#include <string>

BirdController::BirdController(GameObject *gameObject) : Component(gameObject) {
    gameObject->getComponent<PhysicsComponent>()->setLinearVelocity(glm::vec2(2, 0));
}

bool BirdController::onKey(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym) {
            case SDLK_SPACE:
                // doesnt add force of you're at the top of the screen
                if((gameObject->getPosition().y < BirdGame::instance->windowSize.y) && (BirdGame::instance->getGameState() == GameState::Running))
                    jump();
                break;
            default:
                std::cout << "some key pressed" << std::endl;
                break;
        }
    } else if (event.type == SDL_KEYUP){
        switch(event.key.keysym.sym) {
            case SDLK_SPACE:
                jumped = false;
                break;
            default:
                std::cout << "some key released" << std::endl;
                break;
        }
    }
    return false;
}

void BirdController::onCollisionStart(PhysicsComponent *comp) {
    if(comp->getGameObject()->name.find("Wall") != std::string::npos)
        BirdGame::instance->setGameState(GameState::GameOver);
    if(comp->getGameObject()->name.find("Coin") != std::string::npos)
        comp->getGameObject()->removeComponent(comp->getGameObject()->getComponent<SpriteComponent>());
}

void BirdController::onCollisionEnd(PhysicsComponent *comp) {
}

void BirdController::periodicSpeedUp(float deltaTime) {
    if((int)floor(deltaTime) % 5 == 0)
        std::cout << deltaTime << std::endl;
}

void BirdController::jump() {
    if (jumped)
        return;
    gameObject->getComponent<PhysicsComponent>()->addImpulse(glm::vec2(0, 0.2));
    jumped = true;
}


