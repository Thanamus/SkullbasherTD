//
// Created by kb on 31/01/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_ENEMYCOMPONENT_HPP
#define SIMPLERENDERENGINEPROJECT_ENEMYCOMPONENT_HPP

#include "Component.hpp"
#include "Updatable.hpp"
#include "glm/glm.hpp"
#include "health/HealthComponent.hpp"

class Pathfinder;

class EnemyComponent : public HealthComponent, public Updatable {
public:
    explicit EnemyComponent(GameObject* gameObject);
    ~EnemyComponent() override;

    void setWave(int incomingWaveNumber);
    int getWave() const;

    void setEnemyNumber(int incomingEnemyNumber);
    int getEnemyNumber() const;

    void setEnemySetNumber(int incomingEnemySet);
    int getEnemySetNumber() const;

    glm::vec3 getPosition();

    Pathfinder *getPathfinder() const;

    void update(float deltaTime) override;

private:
    Pathfinder* pathfinder;

    //Wave and enemy
    int waveNumber = 0;
    int enemyNumber = 0;
    int enemySetNumber = 0;
};


#endif //SIMPLERENDERENGINEPROJECT_ENEMYCOMPONENT_HPP
