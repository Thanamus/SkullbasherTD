//
// Created by kb on 31/01/2021.
//

#include "EnemyComponent.hpp"
#include "Pathfinder.hpp"
#include "TransformComponent.hpp"
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
    return getGameObject()->getComponent<TransformComponent>()->globalPosition();
}

void EnemyComponent::HandleHealthChange() {
    if (!GameManager::getInstance().levelRunning || gameObject->isQueuedForDeletion())
        return;

    if(this->getHealth() <= 0) {

        std::string  deathSound; 
        gameObject->getComponent<PlaylistComponent>()->getSoundEffectName("death", deathSound);

        SourceManager::Get()->playMyJam(deathSound, this->gameObject->getComponent<TransformComponent>()->position, 20);
        GameManager::getInstance().setTotalEnemies(GameManager::getInstance().getTotalEnemies() - 1);
        gameObject->setQueuedForDeletion(true);
        GameManager::getInstance().getSceneManager()->SpawnCoin(money, this->getGameObject()->getComponent<TransformComponent>()->position);
        if(GameManager::getInstance().getTotalEnemies() <= 0)
        {
            GameManager::getInstance().toggleWinState(true);
        }
    }
    else
    {
        std::string  hitSound;
        gameObject->getComponent<PlaylistComponent>()->getSoundEffectName("hit", hitSound);

        SourceManager::Get()->playMyJam(hitSound, this->gameObject->getComponent<TransformComponent>()->position, 20);
        //play sound or animate, or something
    }
}

float EnemyComponent::getMoney() const {
    return money;
}

void EnemyComponent::setMoney(float money) {
    EnemyComponent::money = money;
}

float EnemyComponent::getDamage() const {
    return damage;
}

void EnemyComponent::setDamage(float damage) {
    EnemyComponent::damage = damage;
}

bool EnemyComponent::isHittable() const {
    return hittable;
}

void EnemyComponent::setHittable(bool hittable_) {
    EnemyComponent::hittable = hittable_;
}
