
#pragma once
#include "GameObject.hpp"
#include "glm/glm.hpp"
#include "../GameManager.hpp"

class Pathfinder {
public:
    explicit Pathfinder(GameObject* _gameObject);

    void update(float deltaTime);

    void fetchNextPathPoint();
    static glm::vec3 previewPathPoint(int pathIndex);

    void setMoving(bool incomingMovingStatus);
    bool isMoving() const;

    const glm::vec3 &getDirection() const;

    int getCurrentPathIndex() const;

    float getMoveSpeed() const;
    void setMoveSpeed(float moveSpeed_);\

    const glm::vec3 &getStartPathPoint() const;
    const glm::vec3 &getNextPathPoint() const;

private:
    GameObject* gameObject;

    int currentPathIndex = 0;
    glm::vec3 direction = glm::vec3(0, 0, 0);
    glm::vec3 startPathPoint = glm::vec3(0, 0, 0);
    glm::vec3 nextPathPoint = glm::vec3(0, 0, 0);
    glm::vec3 currentPosition = glm::vec3(0, 0, 0);
    glm::vec3 nextPosition = glm::vec3(0, 0, 0);
    float moveSpeed = 0.1f;
    bool moving = false;
};
