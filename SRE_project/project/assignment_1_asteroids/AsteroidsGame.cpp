//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//

#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "GameObject.hpp"
#include "SpaceShip.hpp"
#include "AsteroidFactory.hpp"
#include <algorithm>

using namespace sre;
AsteroidsGame *AsteroidsGame::instance = nullptr;

AsteroidsGame *AsteroidsGame::getInstance()
{
    if (!instance)
        instance = new AsteroidsGame();
    return instance;
}

std::vector<std::shared_ptr<GameObject>> AsteroidsGame::getGameObjects() { return gameObjects; };
std::shared_ptr<sre::SpriteAtlas> AsteroidsGame::getAtlas() { return atlas; };
void AsteroidsGame::queueToAdd(std::shared_ptr<GameObject> obj) { gameObjectsToAdd.push_back(obj); };
void AsteroidsGame::queueToDel(std::shared_ptr<GameObject> obj) { gameObjectsToDel.push_back(obj); };

AsteroidsGame::AsteroidsGame()
{
    r.setWindowTitle("Asteroids");

    r.init().withSdlInitFlags(SDL_INIT_EVERYTHING).withSdlWindowFlags(SDL_WINDOW_OPENGL);

    atlas = SpriteAtlas::create("asteroids.json", "asteroids.png");

    camera.setWindowCoordinates();
}

void AsteroidsGame::start()
{
    time_t t; // random seed based on time
    srand((unsigned)time(&t));

    prepGame();

    r.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };

    r.keyEvent = [&](SDL_Event &event) {
        keyEvent(event);
    };

    r.frameRender = [&]() {
        render();
    };

    r.startEventLoop();
}

void AsteroidsGame::incScore()
{
    score++;
}

void AsteroidsGame::prepGame()
{
    gameOver = false;
    gameObjects.clear();
    score = 0;

    auto spaceshipSprite = atlas->get("playerShip1_blue.png");
    auto deadSprite = atlas->get("bang.png");
    gameObjects.push_back(std::make_shared<SpaceShip>(spaceshipSprite, deadSprite));

    for (int i = 0; i < 5; i++)
    {
        AsteroidFactory::getInstance()->makeAsteroid(Asteroid::BIG, Asteroid::GREY);
    }
}

void AsteroidsGame::update(float deltaTime)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->update(deltaTime);
    }
    manageQueues();
}

void AsteroidsGame::manageQueues()
{
    // add elements to be added
    if (gameObjectsToAdd.size() > 0)
    {
        gameObjects.insert(gameObjects.end(), gameObjectsToAdd.begin(), gameObjectsToAdd.end());
        gameObjectsToAdd.clear();
    }

    // remove elements to be deleted
    if (gameObjectsToDel.size() > 0)
    {
        gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [&](const std::shared_ptr<GameObject> x) { return find(gameObjectsToDel.begin(), gameObjectsToDel.end(), x) != gameObjectsToDel.end(); }), gameObjects.end());
        gameObjectsToDel.clear();
    }
}

void drawCircle(std::vector<glm::vec3> &lines, glm::vec2 position, float radius)
{
    float quaterPi = glm::quarter_pi<float>();
    for (float f = 0; f < glm::two_pi<float>(); f += quaterPi)
    {
        // line from
        lines.push_back(glm::vec3{position.x + cosf(f) * radius,
                                  position.y + sinf(f) * radius,
                                  0});
        // line to
        lines.push_back(glm::vec3{position.x + cosf(f + quaterPi) * radius,
                                  position.y + sinf(f + quaterPi) * radius,
                                  0});
    }
}

void AsteroidsGame::render()
{
    auto renderPass = RenderPass::create()
                          .withCamera(camera)
                          .withClearColor(true, {.20, .60, .86, 1})
                          .build();
    auto spriteBatchBuilder = SpriteBatch::create();

    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->render(spriteBatchBuilder);
    }
    auto spriteBatch = spriteBatchBuilder.build();
    renderPass.draw(spriteBatch);

    if (debugCollisionCircles)
    {
        std::vector<glm::vec3> lines;
        for (auto &go : gameObjects)
        {
            auto col = std::dynamic_pointer_cast<Collidable>(go);
            if (col != nullptr)
            {
                drawCircle(lines, go->position, col->getRadius());
            }
        }
        renderPass.drawLines(lines);
    }

    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
    ImGui::LabelText("Score", "%i", score);
    ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event &event)
{
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->onKey(event);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d)
    {
        debugCollisionCircles = !debugCollisionCircles;
    }
    if (gameOver && event.key.keysym.sym == SDLK_SPACE)
    {
        prepGame();
    }
}

void AsteroidsGame::setGameOver()
{
    gameOver = true;
}

int main()
{
    AsteroidsGame::getInstance()->start();
    return 0;
}