#pragma once

#include <vector>
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"

class GameObject;

class AsteroidsGame
{
public:
    static AsteroidsGame *getInstance();
    std::vector<std::shared_ptr<GameObject>> getGameObjects();
    std::shared_ptr<sre::SpriteAtlas> getAtlas();
    void queueToAdd(std::shared_ptr<GameObject> obj);
    void queueToDel(std::shared_ptr<GameObject> obj);
    void manageQueues();
    void start();
    void prepGame();
    void setGameOver();
    void incScore();

private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);

    AsteroidsGame();

    static AsteroidsGame *instance;
    sre::Camera camera;
    sre::SDLRenderer r;
    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<GameObject>> gameObjectsToAdd;
    std::vector<std::shared_ptr<GameObject>> gameObjectsToDel;
    bool debugCollisionCircles = false;
    int score = 0;
    bool gameOver = false;
};
