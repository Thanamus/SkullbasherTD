//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "../Renderable.hpp"
#include "../Updatable.hpp"
#include "glm/glm.hpp"
#include "../../GuiManager.hpp"
#include "../ScheduleManager.hpp"
#include "LevelScene.hpp"
#include "MainMenuScene.hpp"

#include <utility>
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

class GuiManager;
class MainMenuGuiManager;
class LevelGuiManager;

enum DifficultyEnum {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2
};
enum SceneType { LEVEL = 0, MAINMENU = 1};

struct LevelData {
    int id;
    std::string levelName;
    std::string fileName;
    DifficultyEnum difficulty;
    SceneType sceneType;

    LevelData(int id, std::string levelName, std::string fileName, DifficultyEnum difficulty, SceneType sceneType)
    {
        this->id = id;
        this->levelName = std::move(levelName);
        this->fileName = std::move(fileName);
        this->difficulty = difficulty;
        this->sceneType = sceneType;
    }
};

class SceneManager {
public:
    explicit SceneManager();
    virtual ~SceneManager();

    std::shared_ptr<Scene> createScene(std::string levelName);
    void changeScene(std::shared_ptr<LevelData> sceneData);
    void loadMap(std::string filename, std::shared_ptr<Scene> res);
    std::shared_ptr<Scene> createMainMenuScene();

    void setCurrentScene(std::shared_ptr<Scene> res);
    std::shared_ptr<Scene> getCurrentScene();
    std::string getMapsFolderLoc();

    std::shared_ptr<GameManager> gameManager;
    const std::vector<std::shared_ptr<LevelData>> &getLevelsData() const;
private:
    std::vector<std::shared_ptr<LevelData>> levelsData;
    void loadLevelsData();
    //World Map stuff
    // std::string mapAssetFolderLoc = "..\\Assets\\WorldMapAssets"; //didn't work
    std::string mapsFolderLoc = ".\\maps\\"; // apparently does work
    std::string mapAssetFolderLoc = ".\\assets\\worldMapAssets"; // apparently does work
    std::string enemiesAssetFolderLoc = ".\\assets\\enemies";
    std::shared_ptr<Scene> currentScene;
    double tileHeightOffset = -1;
    double tilePosOffset = 1;
    friend GameManager;
    friend MainMenuScene;
    friend MainMenuGuiManager;
    friend LevelGuiManager;
    friend GuiManager;
};

