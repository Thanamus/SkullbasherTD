
#pragma once
#include "GameObject.hpp"
#include "Component.hpp"
#include "Updatable.hpp"

#include "glm/glm.hpp"

class PathFinder : public Component, public Updatable
{
public:
    explicit PathFinder(GameObject *gameobject);
    ~PathFinder();

    void update(float deltaTime) override;

    void fetchNextPathPoint();

    void setWave(int incomingWaveNumber);
    int getWave();

    void setEnemyNumber(int incomingEnemyNumber);
    int getEnemyNumber();

    void setEnemySetNumber(int incomingEnemySet);
    int getEnemySetNumber();

    void setMovingStatus(bool incomingMovingStatus);
    bool getMovingStatus();

private:
    int PathFinder::currentPathIndex = 0;
    glm::vec3 PathFinder::nextPathPoint = glm::vec3(0, 0, 0);
    glm::vec3 PathFinder::currentPosition = glm::vec3(0, 0, 0);
    glm::vec3 PathFinder::nextPosition = glm::vec3(0, 0, 0);
    float PathFinder::moveSpeed = 0.1f; // I think one of these is redundant
    float pfMoveSpeed = 0.1f;

    //Wave and enemy
    int PathFinder::waveNumber = 0;
    int PathFinder::enemyNumber = 0;
    int PathFinder::enemySetNumber = 0;


    bool PathFinder::moving = false;
};
