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
    ~EnemyComponent();

    void HandleHealthChange() override;

    void setWave(int incomingWaveNumber);
    int getWave() const;

    void setEnemyNumber(int incomingEnemyNumber);
    int getEnemyNumber() const;

    void setEnemySetNumber(int incomingEnemySet);
    int getEnemySetNumber() const;

    glm::vec3 getPosition();

    Pathfinder *getPathfinder() const;

    void update(float deltaTime) override;

    float getMoney() const;

    void setMoney(float money);

    float getDamage() const;

    void setDamage(float damage);
private:
    Pathfinder* pathfinder;

    //Wave and enemy
    int waveNumber = 0;
    int enemyNumber = 0;
    int enemySetNumber = 0;
    float money = 0;
    float damage = 0;
    bool hitable = false;
public:
    bool isHitable() const;

    void setHitable(bool hitable);
};


#endif //SIMPLERENDERENGINEPROJECT_ENEMYCOMPONENT_HPP
