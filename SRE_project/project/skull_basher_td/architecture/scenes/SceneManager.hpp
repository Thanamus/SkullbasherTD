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
#include "../rapidjson/document.h"

#include <utility>
#include <vector>
#include <string>


class CameraComponent;
class GameObject;
class Component;
class LightComponent;
class BulletPhysics;
class RigidBodyComponent;
class ScheduleManager;
class GameManager;
class Model;
class Animation;

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

    std::shared_ptr<Scene> createScene();
    void changeScene(const std::shared_ptr<LevelData>& sceneData);
    void loadMap(const std::string& filename, std::shared_ptr<Scene> res);
    static std::shared_ptr<Scene> createMainMenuScene();

    void setCurrentScene(std::shared_ptr<Scene> res);
    std::shared_ptr<Scene> getCurrentScene();
    std::string getMapsFolderLoc();

    const std::vector<std::shared_ptr<LevelData>> &getLevelsData() const;
    void SpawnCoin(float money,glm::vec3 position);
private:
    std::shared_ptr<Model> coinModel;
    std::shared_ptr<Animation> coinAnimation;
    std::vector<std::shared_ptr<LevelData>> levelsData;
    void loadLevelsData();
    void loadLevelsMap(const std::string& filename, std::shared_ptr<Scene> res);
    void loadLevelsEnemies(const std::string& filename, std::shared_ptr<Scene> res);
    void loadLevelsSound(const std::string& filename);
    static float createScaledBounds(float boundSideZero, float boundSideOne , float scale, float factor);
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

    //NB: not sure if this should live here
    glm::vec3 playerSpawnPoint = {0.f,3.f,0.f};
    float playerSpawnRotation;
};

