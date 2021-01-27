//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "Renderable.hpp"
#include "Updatable.hpp"
#include "glm/glm.hpp"
// #include "../GameManager.hpp"
#include "../GuiManager.hpp"
#include "ScheduleManager.hpp"
#include "LevelScene.hpp"
#include "MainMenuScene.hpp"

#include <vector>
#include <string>


class Camera;
class GameObject;
class Component;
class Light;
class BulletPhysics;
class RigidBody;
class ScheduleManager;
class GameManager;

class SceneManager {
public:
    explicit SceneManager();
    ~SceneManager();

    std::shared_ptr<Scene> createScene(std::string levelName);
    void changeScene(std::string levelName);
    void loadMap(std::string filename, std::shared_ptr<Scene> res);
    std::shared_ptr<Scene> createMainMenuScene();

    void setCurrentScene(std::shared_ptr<Scene> res);
    std::shared_ptr<Scene> getCurrentScene();
    std::string getMapsFolderLoc();

    std::shared_ptr<GameManager> gameManager;
private:
    //World Map stuff
    // std::string mapAssetFolderLoc = "..\\Assets\\WorldMapAssets"; //didn't work
    std::string mapsFolderLoc = ".\\maps\\"; // apparently does work
    std::string mapAssetFolderLoc = ".\\assets\\worldMapAssets"; // apparently does work
    std::string enemiesAssetFolderLoc = ".\\assets\\enemies";
    std::shared_ptr<Scene> currentScene;
    double tileHeightOffset = -1;
    double tilePosOffset = 1;
    friend GameManager;
    friend MainMenuGuiManager;
};

