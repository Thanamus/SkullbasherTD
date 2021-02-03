
#pragma once
#include "GameObject.hpp"
#include "glm/glm.hpp"
#include "../GameManager.hpp"

class Pathfinder {
public:
    explicit Pathfinder(GameObject* _gameObject);

    void update(float deltaTime);

    void fetchNextPathPoint();

    void setMoving(bool incomingMovingStatus);
    bool isMoving() const;

    int getCurrentPathIndex() const;

    float getMoveSpeed() const;
    void setMoveSpeed(float moveSpeed_);

private:
    GameObject* gameObject;

    int currentPathIndex = 0;
    glm::vec3 nextPathPoint = glm::vec3(0, 0, 0);
    glm::vec3 currentPosition = glm::vec3(0, 0, 0);
    glm::vec3 nextPosition = glm::vec3(0, 0, 0);
    float moveSpeed = 0.1f;
    bool moving = false;
};
