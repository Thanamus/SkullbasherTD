
#pragma once
#include "GameObject.hpp"
#include "Component.hpp"
#include "Updatable.hpp"

#include "glm/glm.hpp"

class PathFinder : public Component, public Updatable {
public:
    explicit PathFinder(GameObject* gameobject);
    ~PathFinder();

    void update(float deltaTime) override;

    void fetchNextPathPoint();
private:
    int PathFinder::currentPathIndex = 0;
    glm::vec3 PathFinder::nextPathPoint = glm::vec3(0,0,0);
    glm::vec3 PathFinder::currentPosition;
    glm::vec3 PathFinder::nextPosition;
    float PathFinder::moveSpeed = 0.1f;
    float pfMoveSpeed = 0.1f;
};


