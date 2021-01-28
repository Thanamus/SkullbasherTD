//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "SceneManager.hpp"
#include "../Camera.hpp"
#include "../RigidBody.hpp"
#include "../ModelRenderer.hpp"
#include "../Light.hpp"
#include "../BulletPhysics.hpp"

//fps camera stuff
#include "../PersonController.hpp"

//WorldMap Imports
//WorldObject
#include "../WorldObject.hpp"

#include "../PathFinder.hpp"

#include "../SoundEffectsLibrary.hpp"
#include "../SourceManager.hpp"

//rapidjson imports
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "../../LevelGuiManager.hpp"

#include <iostream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

#include <Windows.h>



SceneManager::SceneManager()
{
    loadLevelsData();
}

SceneManager::~SceneManager(){
}

class CustomCollisionHandler : public Component, public CollisionHandler {
public:
    explicit CustomCollisionHandler(GameObject *gameObject) : Component(gameObject) {}

    void onCollision(size_t collisionId, RigidBody *other, glm::vec3 position, bool begin) override {
        if (begin){
            std::cout << "Collision "<< collisionId <<" on "<<other->getGameObject()->getName()<< " at "<<glm::to_string(position)<<std::endl;
        }
    }
    void onCollisionEnd(size_t collisionId) override {
        std::cout << "Collision end "<<collisionId<<std::endl;
    }
};

std::shared_ptr<Scene> SceneManager::createScene(std::string levelName){
    auto res = std::make_shared<LevelScene>();
    auto cameraObj = res->createGameObject("Camera");
    cameraObj->addComponent<Camera>()->clearColor = {0.2,0.2,0.2};
    cameraObj->getComponent<Transform>()->position = {20,1.0f,11};
    cameraObj->getComponent<Transform>()->rotation = {0,190,0};
    cameraObj->addComponent<PersonController>(); // adding the controller to the camera (the player)
    cameraObj->getComponent<PersonController>()->currentScene = res;

    auto sphereObj = res->createGameObject("Sphere");
    auto sphereMR = sphereObj->addComponent<ModelRenderer>();
    sphereMR->setMesh(sre::Mesh::create().withSphere(16,32,0.99f).build());
    sphereObj->getComponent<Transform>()->position = {10,1.0,10};
    sphereObj->getComponent<Transform>()->position = {10,1.0,10};
    sphereObj->addComponent<RigidBody>()->initRigidBodyWithSphere(1, 0);

    auto lightObj = res->createGameObject("Light");
    lightObj->getComponent<Transform>()->rotation = {30,30,0};
    lightObj->addComponent<Light>();

    auto cube = res->createGameObject("Cube");
    cube->getComponent<Transform>()->position = {10,4,10};
    cube->getComponent<Transform>()->rotation = {30,30,10};
    auto cubeRigidBody = cube->addComponent<RigidBody>();
    cubeRigidBody->initRigidBodyWithBox({1,1,1}, 1);

    auto cubeMR = cube->addComponent<ModelRenderer>();
    cubeMR->setMesh(sre::Mesh::create().withCube(0.99).build());
    cube->addComponent<CustomCollisionHandler>();
    auto cubeAN = cube->addComponent<Animator>();
    cubeMR->setAnimator(cubeAN.get());
    auto rotate = std::make_shared<Animation>(true);
    rotate->addFrame(glm::vec3( 0), glm::vec3(1.25), glm::vec3(0), 2.f);
    rotate->addFrame(glm::vec3( 0), glm::vec3(0.8), glm::vec3(0), 2.f);
    cubeAN->addAnimation("rotate", rotate);
    cubeAN->setAnimationState("rotate");

    auto tower = res->createGameObject("Tower");
    tower->getComponent<Transform>()->position = {0,0,0};
    tower->getComponent<Transform>()->rotation = {0,0,0};
    tower->getComponent<Transform>()->scale = {0.2f,0.2f,0.2f};
    auto towerMR = tower->addComponent<ModelRenderer>();
    towerMR->setMesh(sre::Mesh::create().withCube(0.99).build());

    cameraObj->getComponent<PersonController>()->tower = tower;

    auto hand = res->createGameObject("Hand");
    hand->getComponent<Transform>()->position = {0,0,0};
    hand->getComponent<Transform>()->rotation = {0,0,0};
    hand->getComponent<Transform>()->scale = {0.1f,0.1f,0.1f};
    auto handMR = hand->addComponent<ModelRenderer>();
    auto path =  ".\\assets\\lowpoly_crossbow_2_2.obj";
    std::shared_ptr<Model> modelHolder = Model::create().withOBJ(path).withName("hand").build();

    handMR->setMesh(sre::Mesh::create().withCube(0.99).build());
    handMR->setModel(modelHolder);

    cameraObj->getComponent<PersonController>()->hand = hand;

    return res;
};

std::shared_ptr<Scene> SceneManager::createMainMenuScene(){
    auto res = std::make_shared<MainMenuScene>();
    auto cameraObj = res->createGameObject("Camera");
    cameraObj->addComponent<Camera>()->clearColor = {0.2,0.2,0.2};
    cameraObj->getComponent<Transform>()->position = {20,1.0f,11};
    cameraObj->getComponent<Transform>()->rotation = {0,190,0};

    auto lightObj = res->createGameObject("Light");
    lightObj->getComponent<Transform>()->rotation = {30,30,0};
    lightObj->addComponent<Light>();

    return res;
};

void SceneManager::setCurrentScene(std::shared_ptr<Scene> res){
    this->currentScene = res;
};

std::shared_ptr<Scene> SceneManager::getCurrentScene(){
    return currentScene;
};

std::string SceneManager::getMapsFolderLoc(){
    return mapsFolderLoc;
};

void SceneManager::loadMap(std::string filename, std::shared_ptr<Scene> res){
    using namespace rapidjson;
    std::ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    //init a map row to temporarily hold the map row
    std::vector<int> mapRow;

    mapRow.clear(); //because mapRow is used in the original

    std::vector<glm::vec3> pathHolder;

    int rowArrayCount = d["tileMap"].GetArray().Size(); //get the tile map key, the array size is the outset loop's size
    int tileTypeInt;
    std::string tileTypeStr;
    float rotationHolder;
    glm::vec3 positionHolder;
    glm::vec3 scaleHolder;// scale

    bool isbuildableHolder = false;
    bool isPathHolder = false;

    std::string modelName = "";
    std::shared_ptr<Model> modelHolder;
    std::shared_ptr<sre::Mesh> meshHolder;
    std::shared_ptr<sre::Material> matHolder;
    std::vector<std::shared_ptr<sre::Material>> materialsLoaded;

    std::vector<glm::vec3> pathBuffer;
    bool reversePathBuffer = false;

    for (size_t row = 0; row < rowArrayCount; row++) //go through each 'row' of the map
    {
 
        mapRow.clear(); //clear the temporary buffer
        int heightArrayCount = d["tileMap"][row].GetArray().Size(); //inner array is the number of positions in the current row
        pathBuffer.clear();
        // push each field's value into the buffer
        for (size_t heightFactor = 0; heightFactor < heightArrayCount; heightFactor++)
        {
            double tileHeight = tileHeightOffset + heightFactor;

            //variables defined here, attempting to save processing power
            
            int columnArrayCount = d["tileMap"][row].GetArray()[heightFactor].GetArray().Size();
            for (size_t column = 0; column < columnArrayCount; column++)
            {
                tileTypeInt = d["tileMap"].GetArray()[row].GetArray()[heightFactor].GetArray()[column].GetInt();

                if (tileTypeInt != 0)
                {
                    tileTypeStr = std::to_string(tileTypeInt);
                    const char *c = tileTypeStr.c_str(); 

                    //get position and rotation of the block
                    rotationHolder = d["MapLookup"][c]["rotation"].GetFloat();
                    positionHolder = glm::vec3(row+tilePosOffset,tileHeight,column+tilePosOffset);

                    scaleHolder.x = d["MapLookup"][c]["scaleFactors"]["x"].GetFloat();
                    scaleHolder.y = d["MapLookup"][c]["scaleFactors"]["y"].GetFloat();
                    scaleHolder.z = d["MapLookup"][c]["scaleFactors"]["z"].GetFloat();

                    isbuildableHolder = d["MapLookup"][c]["isbuildable"].GetBool();
                    isPathHolder = d["MapLookup"][c]["isPath"].GetBool();

                    modelName = d["MapLookup"][c]["object"].GetString();
                    
                    //Load the mesh from file
                    auto filePath = mapAssetFolderLoc + "\\" + modelName;
                    modelHolder = Model::create().withOBJ(filePath).withName(modelName).build();

                    //create game object
                    auto mapTile = res->createGameObject(modelName);
                    auto mapTileMR = mapTile->addComponent<ModelRenderer>();
                    mapTile->addComponent<WorldObject>();
                    mapTile->getComponent<WorldObject>()->setBuildable(isbuildableHolder);
                    mapTile->getComponent<WorldObject>()->setIsPath(isPathHolder);

                    mapTileMR->setModel(modelHolder);
                    float xOffset = d["MapLookup"][c]["posOffset"]["x"].GetFloat();
                    float yOffset = d["MapLookup"][c]["posOffset"]["y"].GetFloat();;
                    float zOffset = d["MapLookup"][c]["posOffset"]["z"].GetFloat();;

                    positionHolder.x += xOffset;
                    positionHolder.y += yOffset;
                    positionHolder.z += zOffset;

                    mapTile->getComponent<Transform>()->position = positionHolder;
                    mapTile->getComponent<Transform>()->rotation.y = rotationHolder;

                    mapTile->getComponent<Transform>()->scale = scaleHolder;
                    auto bounds = mapTileMR->getMesh()->getBoundsMinMax();
                    
                    float length = (fabs(bounds[0].z) + fabs(bounds[1].z))/4;
                    float width = (fabs(bounds[0].x) + fabs(bounds[1].x))/4;
                    float height = (fabs(bounds[0].y) + fabs(bounds[1].y))/4;

                    mapTile->addComponent<RigidBody>()->initRigidBodyWithBox({length, height, width}, 0);

                    if (isPathHolder)
                    {
                        pathBuffer.push_back(positionHolder);

                        //check if reverseBuffer should be set
                        reversePathBuffer = d["MapLookup"][c]["reversePathBuffer"].GetBool(); //a 'reverseBuffer' parameter tells the game that the path section needs to be loaded in reverse
                    }
                    
                    
                }
            }
            
        }

        //if the path is running from 'right to left' in the map, then it needs to be loaded into the path in reverse
        if (!reversePathBuffer && !pathBuffer.empty())
        {
            for (size_t pathInBuffer = 0; pathInBuffer <= pathBuffer.size()-1; pathInBuffer++)
            {
                pathHolder.push_back(pathBuffer[pathInBuffer]);
            }
        } else if (!pathBuffer.empty()) {
            for (size_t pathInBuffer = pathBuffer.size(); pathInBuffer > 0; pathInBuffer--)
            {
                pathHolder.push_back(pathBuffer[pathInBuffer-1]);
            }
        }
        reversePathBuffer = false;
    }
    gameManager->setPath(pathHolder);

    //-----------------Load enemies----------------------
    int howManyWaves = d["waves"].GetArray().Size();
    int enemyTypeInt;
    std::string enemyTypeStr;

    for (size_t wave = 0; wave < howManyWaves; wave++)
    { //wave level
        //get the number of enemyTypes and quantities in order
        int numberOfEnemySets = d["waves"][wave]["enemies"].GetArray().Size();
        std::vector<enemySetsInWave> enemySetsHolder;
        enemySetsInWave tempEnemySet;

        //wave schedule parameters per this current wave
        waveScheduleDetails waveScheduleDetailHolder;
        waveScheduleDetailHolder.timeBetweenWaves = d["waves"][wave]["timeBetweenWaves"].GetInt();
        waveScheduleDetailHolder.timeBetweenEnemies = d["waves"][wave]["timeBetweenEnemy"].GetInt();

        for (size_t currentEnemySet = 0; currentEnemySet < numberOfEnemySets; currentEnemySet++)
        { //inside set level
            enemyTypeInt = d["waves"][wave]["enemies"][currentEnemySet]["enemyType"].GetInt();
            enemyTypeStr = std::to_string(enemyTypeInt);
            const char *enemyTypeChar = enemyTypeStr.c_str();

            int howManyOfEnemyType = d["waves"][wave]["enemies"][currentEnemySet]["quantity"].GetInt();
            rotationHolder = d["enemyLookup"][enemyTypeChar]["rotation"].GetFloat();

            //put the set into the enemySetHolder to be sent to GameManager later
            tempEnemySet.enemyType = enemyTypeInt;
            tempEnemySet.quantiy = howManyOfEnemyType;
            enemySetsHolder.push_back(tempEnemySet);

            std::vector<glm::vec3> path = gameManager->getPath();
            int endOfPath = path.size();
            positionHolder = path[endOfPath-1]; //sets where the enemy will spawn
            positionHolder.y += 1.0; //compensates for the fact that the path is well, the ground

            scaleHolder.x = d["enemyLookup"][enemyTypeChar]["scaleFactors"]["x"].GetFloat();// scale
            scaleHolder.y = d["enemyLookup"][enemyTypeChar]["scaleFactors"]["y"].GetFloat();// scale
            scaleHolder.z = d["enemyLookup"][enemyTypeChar]["scaleFactors"]["z"].GetFloat();// scale

            modelName = d["enemyLookup"][enemyTypeChar]["object"].GetString();
            
            //Load the mesh from file
            auto filePath = enemiesAssetFolderLoc + "\\" + modelName;
            modelHolder = Model::create().withOBJ(filePath).withName(modelName).build();
            std::cout << "Asset folder: " << filePath << "\n";
            std::cout << "Model Name: " << modelName << "\n";

            // //create the world object map tile
            for (int anEnemy = 0; anEnemy < howManyOfEnemyType; anEnemy++)
            {
                //create game object
                auto enemy = res->createGameObject(modelName);
                auto enemyMR = enemy->addComponent<ModelRenderer>();
                enemyMR->setModel(modelHolder);

                enemy->getComponent<Transform>()->position = positionHolder;
                enemy->getComponent<Transform>()->rotation.y = rotationHolder;
                enemy->getComponent<Transform>()->scale = scaleHolder;
                auto bounds = enemyMR->getMesh()->getBoundsMinMax();

                //Add Path Finder to Skull
                enemy->addComponent<PathFinder>();
                enemy->getComponent<PathFinder>()->setEnemyNumber(anEnemy);
                enemy->getComponent<PathFinder>()->setWave(wave);
                enemy->getComponent<PathFinder>()->setEnemySetNumber(currentEnemySet);
                std::cout << "created enemy with enemy number: " << anEnemy << std::endl; 
                std::cout << "created enemy with set number: " << currentEnemySet << std::endl; 
                std::cout << "created enemy with wave number: " << wave << std::endl; 
            }
        }

        //send the wave details to the Game Manager
        gameManager->addWave(wave, enemySetsHolder, waveScheduleDetailHolder);
    }
    
    // ----------------- LOAD SOUNDS --------------------------
    int howManySounds = d["soundEffects"].GetArray().Size();
    auto mySoundEffectsLibrary = SoundEffectsLibrary::Get();

    for (size_t sound = 0; sound < howManySounds; sound++){
        //Go through the list of sounds and add them to the soundsEffectsLibrary
        std::string soundStr = d["soundEffects"][sound]["location"].GetString();
        const char *soundChar = soundStr.c_str();
        mySoundEffectsLibrary->Load(soundChar);
    }
}

void SceneManager::changeScene(std::string levelName) {
    std::string path = ".\\maps\\";
    path.append(levelName);
    auto scene = createScene(path);
    setCurrentScene(scene);
    getCurrentScene()->guiManager = std::make_shared<LevelGuiManager>(gameManager);
    getCurrentScene()->gameManager = gameManager;
    getCurrentScene()->sceneManager = static_cast<const std::shared_ptr<SceneManager>>(this);
    gameManager->currentScene = getCurrentScene();
    loadMap(path, getCurrentScene());

    auto scheduleManager = std::make_shared<ScheduleManager>();
    scheduleManager->currentScene = getCurrentScene(); //not sure about this pattern, here the two managers 'know' each other
    getCurrentScene()->scheduleManager = scheduleManager;

    gameManager->setInitialWaveStats();
    scheduleManager->fetchInitialWaveSchedule();
}

const std::vector<std::shared_ptr<LevelData>> &SceneManager::getLevelsData() const {
    return levelsData;
}

void SceneManager::loadLevelsData() {

    std::string pattern(".\\maps\\*.json*");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    std::cout << "--Reading Level Files--" << std::endl;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            std::cout << "Reading: " << data.cFileName << std::endl;
            using namespace rapidjson;
            std::string wholePath = ".\\maps\\";
            wholePath.append(data.cFileName);
            std::ifstream fis(wholePath);
            IStreamWrapper isw(fis);
            Document d;
            d.ParseStream(isw);

            auto levelId = d["levelId"].GetFloat(); //inner array is the number of positions in the current row
            auto levelName = d["levelName"].GetString(); //inner array is the number of positions in the current row
            auto levelDifficulty = d["levelDifficulty"].GetInt(); //inner array is the number of positions in the current row
            auto difficultyEnum = static_cast<DifficultyEnum>(levelDifficulty);

            levelsData.push_back(std::make_shared<LevelData>(levelId,levelName, data.cFileName, difficultyEnum));
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

#pragma clang diagnostic pop