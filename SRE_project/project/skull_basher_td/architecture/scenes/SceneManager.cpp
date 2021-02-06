//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "SceneManager.hpp"
#include "../Camera.hpp"
#include "../physics/RigidBody.hpp"
#include "../physics/GhostObject.hpp"

#include "../ModelRenderer.hpp"
#include "../Light.hpp"
#include "../physics/BulletPhysics.hpp"

//fps camera stuff
#include "../PersonController.hpp"

//WorldMap Imports
//WorldObject
#include "../WorldObject.hpp"

#include "../PathFinder.hpp"

#include "../sound/SoundEffectsLibrary.hpp"
#include "../sound/SourceManager.hpp"

//rapidjson imports
#include "../rapidjson/istreamwrapper.h"
#include "../../LevelGuiManager.hpp"

// Collision imports
#include "../collisions/EnemyCollisionHandler.hpp"
#include "../collisions/PlayerCollisionHandler.hpp"
#include "../health/EnemyHealth.hpp"

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

std::shared_ptr<Scene> SceneManager::createScene(std::string levelName){
    auto res = std::make_shared<LevelScene>();
    auto cameraObj = res->createGameObject("Camera");
    cameraObj->addComponent<Camera>()->clearColor = {0.2,0.2,0.2};
    cameraObj->getComponent<Transform>()->position = playerSpawnPoint;
    cameraObj->getComponent<Transform>()->rotation = {0,190,0};
    cameraObj->addComponent<RigidBody>()->initRigidBodyWithSphere(0.6f, 1, PLAYER, BUILDINGS | ENEMIES | CRYSTAL | COINS); // Dynamic physics object

    //--- end setting cameras

    cameraObj->addComponent<PersonController>();
    cameraObj->addComponent<PlayerCollisionHandler>();

    auto lightObj = res->createGameObject("Light");
    lightObj->getComponent<Transform>()->rotation = {30,30,0};
    lightObj->addComponent<Light>();

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
    auto path =  ".\\assets\\lowpoly_crossbow_2_5.obj";
    std::shared_ptr<Model> modelHolder = Model::create().withOBJ(path).withName("hand").build();

    handMR->setMesh(sre::Mesh::create().withCube(0.99).build());
    handMR->setModel(modelHolder);

    cameraObj->getComponent<PersonController>()->hand = hand;

    auto crystal = res->createGameObject("Crystal");
    crystal->getComponent<Transform>()->position = {2,0,4};
    crystal->getComponent<Transform>()->rotation = {0,0,0};
    crystal->getComponent<Transform>()->scale = {0.4f,0.4f,0.4f};
    crystal->addComponent<CrystalHealth>();
    crystal->getComponent<CrystalHealth>()->setHealth(100);
    auto crystalMR = crystal->addComponent<ModelRenderer>();
    auto crystalPath =  ".\\assets\\crystal.obj";
    //TODO: review animation
    auto crystalAN = crystal->addComponent<Animator>();
    crystalMR->setAnimator(crystalAN.get());

    auto crystalRotate = std::make_shared<Animation>(true);
    crystalRotate->addFrame(glm::vec3( 0), glm::vec3(1), glm::vec3(0), 5.f);
    crystalRotate->addFrame(glm::vec3( 0), glm::vec3(1), glm::vec3(360), 5.f);
    crystalAN->addAnimation("rotate", crystalRotate);
    crystalAN->setAnimationState("rotate");

    auto crystalRigidBody = crystal->addComponent<RigidBody>();
    // ---- set crystal collision group and flags
    crystalRigidBody->initRigidBodyWithSphere(0.5f, 1, CRYSTAL, ENEMIES); // crystal needs to be sphere -> skull collision only works with box
    
    // ---- making sure that crystal can't move if hit
    crystalRigidBody->getRigidBody()->setAngularFactor(btVector3(0,0,0));
    crystalRigidBody->getRigidBody()->setLinearFactor(btVector3(0,0,0));

    GameManager::getInstance().crystal = crystal->getComponent<CrystalHealth>();

    crystalMR->setModel(Model::create().withOBJ(crystalPath).withName("crystal").build());

    res->SpawnCoin({4,0,4});

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
    loadLevelsMap(filename, res);
    loadLevelsEnemies(filename, res);
    loadLevelsSound(filename);
}

void SceneManager::changeScene(std::shared_ptr<LevelData> sceneData) {
    std::string path = ".\\maps\\";
    path.append(sceneData->fileName);

    if(sceneData->sceneType == 0)
    {
        GameManager::getInstance().init();
        auto scene = createScene(path);
        setCurrentScene(scene);
        getCurrentScene()->guiManager = std::make_shared<LevelGuiManager>();
        loadMap(path, getCurrentScene());

        auto scheduleManager = std::make_shared<ScheduleManager>();
        getCurrentScene()->scheduleManager = scheduleManager;

        GameManager::getInstance().setInitialWaveStats();
        scheduleManager->fetchInitialWaveSchedule();
        GameManager::getInstance().ToggleLockMouse();
    }
    else
    {
        GameManager::getInstance().init();
        auto scene = createMainMenuScene();
        setCurrentScene(scene);
        getCurrentScene()->guiManager = std::make_shared<MainMenuGuiManager>();
    }
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

            auto sceneType = d["sceneType"].GetInt(); //inner array is the number of positions in the current row
            auto sceneTypeEnum = static_cast<SceneType>(sceneType);

            levelsData.push_back(std::make_shared<LevelData>(levelId,levelName, data.cFileName, difficultyEnum, sceneTypeEnum));
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

float SceneManager::createScaledBounds(float boundSideZero, float boundSideOne, float scale, float factor) {
    return (fabs(boundSideZero + (scale * 2)) + fabs(boundSideOne + (scale * 2)))/factor;
}

void SceneManager::loadLevelsSound(std::string filename) {
    using namespace rapidjson;
    std::ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

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

void SceneManager::loadLevelsMap(std::string filename, std::shared_ptr<Scene> res) {
    using namespace rapidjson;
    std::ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

// --------------- set player spawn point
    float spawnPointX = 0.f;
    float spawnPointY = 0.f;
    float spawnPointZ = 0.f;

    spawnPointX = d["playerSpawnPoint"].GetArray()[0].GetFloat();
    spawnPointY = d["playerSpawnPoint"].GetArray()[1].GetFloat();
    spawnPointZ = d["playerSpawnPoint"].GetArray()[2].GetFloat();
    playerSpawnPoint = {spawnPointX,spawnPointY,spawnPointZ};

    playerSpawnRotation = d["playerSpawnRotation"].GetFloat();
    std::cout << " spawnPointRotation should be: " << playerSpawnRotation << std::endl;

    auto tempCam = currentScene->cameras[0]->getGameObject();
    tempCam->getComponent<Transform>()->position = playerSpawnPoint;
    tempCam->getComponent<Transform>()->rotation.y = playerSpawnRotation;

    glm::vec3 glmCameraPosition =  tempCam->getComponent<Transform>()->position;
    btTransform transform;
    btVector3 btCameraPosition = {glmCameraPosition.x, glmCameraPosition.y, glmCameraPosition.z};
    transform.setOrigin(btCameraPosition);

    std::cout << " spawnPointRotation is actually:  " << tempCam->getComponent<Transform>()->rotation.y << std::endl;

    glm::quat inputQuat = glm::quat(tempCam->getComponent<Transform>()->rotation);
    btQuaternion btInputQuat = {inputQuat.x, -inputQuat.y, inputQuat.z, inputQuat.w,};
    transform.setRotation(btInputQuat);

    tempCam->getComponent<RigidBody>()->getRigidBody()->setWorldTransform(transform);

// ------------------- end setting player Spawn point

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
    glm::vec3 collisionHolder;

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

                    scaleHolder.x = d["MapLookup"][c]["scaleFactors"]["x"].GetFloat();
                    scaleHolder.y = d["MapLookup"][c]["scaleFactors"]["y"].GetFloat();
                    scaleHolder.z = d["MapLookup"][c]["scaleFactors"]["z"].GetFloat();

                    positionHolder = glm::vec3((row * (scaleHolder.x * 2)) + tilePosOffset,tileHeight * (scaleHolder.y * 2),(column * (scaleHolder.z * 2))+ tilePosOffset);

                    isbuildableHolder = d["MapLookup"][c]["isbuildable"].GetBool();
                    isPathHolder = d["MapLookup"][c]["isPath"].GetBool();

                    modelName = d["MapLookup"][c]["object"].GetString();

                    //Load the mesh from file
                    auto filePath = mapAssetFolderLoc + "\\" + modelName;
                    //auto filePath = ".\\assets\\skull.obj";
                    modelHolder = Model::create().withOBJ(filePath).withName(modelName).build();

                    //create game object
                    auto mapTile = res->createGameObject(modelName);
                    auto mapTileMR = mapTile->addComponent<ModelRenderer>();
                    mapTile->addComponent<WorldObject>();
                    mapTile->getComponent<WorldObject>()->setBuildable(isbuildableHolder);
                    mapTile->getComponent<WorldObject>()->setIsPath(isPathHolder);

                    // NEW
                    mapTileMR->setModel(modelHolder);
                    float xOffset = d["MapLookup"][c]["posOffset"]["x"].GetFloat();
                    float yOffset = d["MapLookup"][c]["posOffset"]["y"].GetFloat();
                    float zOffset = d["MapLookup"][c]["posOffset"]["z"].GetFloat();

                    positionHolder.x += xOffset;
                    positionHolder.y += yOffset;
                    positionHolder.z += zOffset;

                    mapTile->getComponent<Transform>()->position = positionHolder;
                    mapTile->getComponent<Transform>()->rotation.y = rotationHolder;

                    mapTile->getComponent<Transform>()->scale = scaleHolder;
                    auto bounds = mapTileMR->getMesh()->getBoundsMinMax();

                    collisionHolder.x = d["MapLookup"][c]["collision"]["x"].GetFloat();
                    collisionHolder.y = d["MapLookup"][c]["collision"]["y"].GetFloat();
                    collisionHolder.z = d["MapLookup"][c]["collision"]["z"].GetFloat();

                    float length = createScaledBounds(bounds[0].z, bounds[1].z, collisionHolder.z, 4);
                    float width = createScaledBounds(bounds[0].x, bounds[1].x, collisionHolder.x, 4);
                    float height = createScaledBounds(bounds[0].y, bounds[1].y, collisionHolder.y, 4);

                    mapTile->addComponent<RigidBody>()->initRigidBodyWithBox({length, height, width}, 0, BUILDINGS, PLAYER);

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
    GameManager::getInstance().setPath(pathHolder);
    auto crystal = GameManager::getInstance().crystal->getGameObject();
    auto path = GameManager::getInstance().getPath();
    crystal->getComponent<Transform>()->position = path[0];
}

void SceneManager::loadLevelsEnemies(std::string filename, std::shared_ptr<Scene> res) {
    using namespace rapidjson;
    std::ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    //-----------------Load enemies----------------------
    int howManyWaves = d["waves"].GetArray().Size();
    int enemyTypeInt;
    std::string enemyTypeStr;
    float rotationHolder;
    glm::vec3 positionHolder;
    glm::vec3 collisionHolder;
    glm::vec3 scaleHolder;// scale
    std::string modelName = "";
    std::shared_ptr<Model> modelHolder;
    std::shared_ptr<sre::Mesh> meshHolder;
    std::shared_ptr<sre::Material> matHolder;
    std::vector<std::shared_ptr<sre::Material>> materialsLoaded;

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
            GameManager::getInstance().setTotalEnemies(GameManager::getInstance().getTotalEnemies() + howManyOfEnemyType);
            enemySetsHolder.push_back(tempEnemySet);


            std::vector<glm::vec3> path = GameManager::getInstance().getPath();
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

            float moveSpeedToBe = d["enemyLookup"][enemyTypeChar]["moveSpeed"].GetFloat();
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

                collisionHolder.x = d["enemyLookup"][enemyTypeChar]["collision"]["x"].GetFloat();
                collisionHolder.y = d["enemyLookup"][enemyTypeChar]["collision"]["y"].GetFloat();
                collisionHolder.z = d["enemyLookup"][enemyTypeChar]["collision"]["z"].GetFloat();

                float length = createScaledBounds(bounds[0].z, bounds[1].z, collisionHolder.z, 7);
                float width = createScaledBounds(bounds[0].x, bounds[1].x, collisionHolder.x, 7);
                float height = createScaledBounds(bounds[0].y, bounds[1].y, collisionHolder.y, 7);

                // collisions work properly if Skull has a box shape, it's weird, but sphere's don't work
                enemy->addComponent<RigidBody>()->initRigidBodyWithBox({length,width,height}, 1, ENEMIES,  PLAYER | CRYSTAL); // mass of 0 sets the rigidbody as kinematic (or static)

                // overriding gravity, not 100% it's needed now or not
                enemy->getComponent<RigidBody>()->getRigidBody()->setGravity({0,0,0});

                //Add Path Finder to Skull
                enemy->addComponent<PathFinder>();
                enemy->getComponent<PathFinder>()->setEnemyNumber(anEnemy);
                enemy->getComponent<PathFinder>()->setWave(wave);
                enemy->getComponent<PathFinder>()->setEnemySetNumber(currentEnemySet);
                enemy->getComponent<PathFinder>()->setMoveSpeed(moveSpeedToBe);
                std::cout << "created enemy with enemy number: " << anEnemy << std::endl;
                std::cout << "created enemy with set number: " << currentEnemySet << std::endl;
                std::cout << "created enemy with wave number: " << wave << std::endl;
                enemy->addComponent<EnemyCollisionHandler>();

                enemy->addComponent<EnemyHealth>();
                enemy->getComponent<EnemyHealth>()->addHealth(d["enemyLookup"][enemyTypeChar]["health"].GetFloat());
            }
        }

        //send the wave details to the Game Manager
        GameManager::getInstance().addWave(wave, enemySetsHolder, waveScheduleDetailHolder);
    }
}

#pragma clang diagnostic pop