//
// Created by kb on 31/01/2021.
//

#include "EnemyComponent.hpp"
#include "Pathfinder.hpp"
#include "Transform.hpp"
#include "sound/SourceManager.hpp"
#include "./sound/PlaylistComponent.hpp"

EnemyComponent::EnemyComponent(GameObject* gameObject) : HealthComponent(gameObject) {
    pathfinder = new Pathfinder(gameObject);
}

EnemyComponent::~EnemyComponent() {
    delete pathfinder;
}

void EnemyComponent::update(float deltaTime) {
    pathfinder->update(deltaTime);
}

void EnemyComponent::setWave(int incomingWaveNumber){
    waveNumber = incomingWaveNumber;
}

int EnemyComponent::getWave() const{
    return waveNumber;
}

void EnemyComponent::setEnemyNumber(int incomingEnemyNumber){
    enemyNumber = incomingEnemyNumber;
}

int EnemyComponent::getEnemyNumber() const{
    return enemyNumber;
}

void EnemyComponent::setEnemySetNumber(int incomingEnemySet){
    enemySetNumber = incomingEnemySet;
}

int EnemyComponent::getEnemySetNumber() const {
    return enemySetNumber;
}

Pathfinder *EnemyComponent::getPathfinder() const {
    return pathfinder;
}

glm::vec3 EnemyComponent::getPosition() {
    return getGameObject()->getComponent<Transform>()->globalPosition();
}

void EnemyComponent::HandleHealthChange() {
    if (!GameManager::getInstance().levelRunning)
        return;

    if(this->getHealth() <= 0) {

        std::string  deathSound; 
        gameObject->getComponent<PlaylistComponent>()->getSoundEffectName("death", &deathSound);

        // SourceManager::Get()->playMyJam("deathd.wav", this->gameObject->getComponent<Transform>()->position, 20);
        SourceManager::Get()->playMyJam(deathSound, this->gameObject->getComponent<Transform>()->position, 20);
        gameObject->setQueuedForDeletion(true);
    }
    else
    {
        //play sound or animate, or something
    }
}
