
#pragma once
#include "GameObject.hpp"
#include "glm/glm.hpp"

class Pathfinder {
public:
    explicit Pathfinder(GameObject* _gameObject);

    void update(float deltaTime);

    void fetchNextPathPoint();

    void setMoving(bool incomingMovingStatus);
    bool isMoving() const;

    int getCurrentPathIndex() const;
private:
    std::shared_ptr<GameManager> gameManager;
    GameObject* gameObject;

    int currentPathIndex = 0;
    glm::vec3 nextPathPoint = glm::vec3(0, 0, 0);
    glm::vec3 currentPosition = glm::vec3(0, 0, 0);
    glm::vec3 nextPosition = glm::vec3(0, 0, 0);
    float moveSpeed = 0.1f; // I think one of these is redundant
    float pfMoveSpeed = 0.1f;

    bool moving = false;
};
