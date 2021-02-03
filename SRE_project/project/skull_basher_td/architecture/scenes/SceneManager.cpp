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

#include "../Pathfinder.hpp"

#include "../SoundEffectsLibrary.hpp"
#include "../SourceManager.hpp"

//rapidjson imports
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "../../LevelGuiManager.hpp"
#include "../TowerBehaviourComponent.hpp"
#include "../health/HealthComponent.hpp"
#include "../health/CrystalHealth.hpp"
#include "../CustomerCollisionHandler.hpp"
#include "../EnemyCollisionHandler.hpp"

#include <iostream>
#include <utility>

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
    cameraObj->getComponent<Transform>()->position = {20,3.0f,11};
    cameraObj->getComponent<Transform>()->rotation = {0,190,0};
    cameraObj->addComponent<RigidBody>()->initRigidBodyWithSphere(0.6f, 1); // Dynamic physics object

    //---- setting cameras? // kinda syncs the physics world and the transform
    glm::vec3 glmCameraPosition =  cameraObj->getComponent<Transform>()->position;
    btTransform transform;
    btVector3 btCameraPosition = {glmCameraPosition.x, glmCameraPosition.y, glmCameraPosition.z};
    transform.setOrigin(btCameraPosition);

    glm::quat inputQuat = glm::quat(cameraObj->getComponent<Transform>()->rotation);
    btQuaternion btInputQuat = {inputQuat.x, -inputQuat.y, inputQuat.z, inputQuat.w,};
    transform.setRotation(btInputQuat);

    cameraObj->getComponent<RigidBody>()->getRigidBody()->setWorldTransform(transform);

    //--- end setting cameras

    cameraObj->addComponent<PersonController>();
    cameraObj->getComponent<PersonController>()->currentScene = res;

    auto lightObj = res->createGameObject("Light");
    lightObj->getComponent<Transform>()->rotation = {30,30,0};
    lightObj->addComponent<Light>();

    auto tower = res->createGameObject("Tower");
    tower->getComponent<Transform>()->position = {0,0,0};
    tower->getComponent<Transform>()->rotation = {0,0,0};
    tower->getComponent<Transform>()->scale = {0.2f,0.2f,0.2f};
    auto towerMR = tower->addComponent<ModelRenderer>();
    towerMR->setMesh(sre::Mesh::create().withCube(0.99).build());
    auto towerTB = tower->addComponent<TowerBehaviourComponent>();
    towerTB->setEnabled(false);

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

    auto crystal = res->createGameObject("Crystal");
    crystal->getComponent<Transform>()->position = {2,0,4};
    crystal->getComponent<Transform>()->rotation = {0,0,0};
    crystal->getComponent<Transform>()->scale = {0.4f,0.4f,0.4f};
    crystal->addComponent<CrystalHealth>();
    crystal->getComponent<CrystalHealth>()->setHealth(100);
    auto crystalMR = crystal->addComponent<ModelRenderer>();
    auto crystalPath =  ".\\assets\\crystal.obj";
    auto crystalAN = crystal->addComponent<Animator>();
    crystal->getComponent<Transform>()->setAnimator(crystalAN);
    crystal->getComponent<Transform>()->setModelRenderer(crystalMR);
    auto crystalRotate = std::make_shared<Animation>(true);
    crystalRotate->addFrame(glm::vec3( 0), glm::vec3(0), glm::vec3(0), 5.f);
    crystalRotate->addFrame(glm::vec3( 0), glm::vec3(0), glm::vec3(360), 5.f);
    crystalAN->addAnimation("rotate", crystalRotate);
    crystalAN->setAnimationState("rotate");

    auto crystalRigidBody = crystal->addComponent<RigidBody>();
    crystalRigidBody->initRigidBodyWithBox({0.5,1.5,0.5}, 0);

    crystalMR->setModel(Model::create().withOBJ(crystalPath).withName("crystal").build());

    gameManager->crystal = crystal->getComponent<CrystalHealth>();

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
    this->currentScene = std::move(res);
};

std::shared_ptr<Scene> SceneManager::getCurrentScene(){
    return currentScene;
};

std::string SceneManager::getMapsFolderLoc(){
    return mapsFolderLoc;
};

void SceneManager::loadMap(std::string filename, std::shared_ptr<Scene> res){
    // original
    // push back one row of tiles (a vector)
    // adding another vector (to make a vector of vectors), adds another whole row of field values

    // values.push_back( {0, 0, 0,0} ); // -------> x
    // values.push_back( {0,-1,-1,2} ); // |
    // values.push_back( {0,-1,-1,9} ); // |
    // values.push_back( {0, 0, 0,0} ); // v z

    // std::cout << "trying to load json: " << filename << "\n";

    using namespace rapidjson;
    std::ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    //Hardcoded start position // Original
    // startingPosition.x = 1.5;
    // startingPosition.y = 1.5;
    // startingRotation = 22.5;

    //JSON start position
    // startingPosition.x = d["spawn"]["x"].GetFloat();
    // startingPosition.y = d["spawn"]["y"].GetFloat();
    // startingRotation = d["spawn"]["angle"].GetFloat();

    //init a map row to temporarily hold the map row
    std::vector<int> mapRow;

    // ifstream fis2("SkullBasherTDLevel0.json");
    // IStreamWrapper isw2(fis2);
    // Document d2;
    // d2.ParseStream(isw2);

    // std::vector<int> mapRow;
    mapRow.clear(); //because mapRow is used in the original

    std::vector<glm::vec3> pathHolder;

    int rowArrayCount = d["tileMap"].GetArray().Size(); //get the tile map key, the array size is the outset loop's size
    // std::cout << "rowArrayCount: " << rowArrayCount << "\n";
    int tileTypeInt;
    std::string tileTypeStr;
    float rotationHolder;
    glm::vec3 positionHolder;
    glm::vec3 scaleHolder;// scale

    bool isbuildableHolder = false;
    bool isPathHolder = false;

    std::string modelName;
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
        // std::cout << "inner array count:" << innerArrayCount << "\n";
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
                    // meshHolder = sre::ModelImporter::importObj(".\\Assets\\WorldMapAssets", "Floor01.obj", materialsLoaded);
                    // OLD: meshHolder = sre::ModelImporter::importObj(mapAssetFolderLoc, modelName, materialsLoaded);
                    // NEW
                    auto filePath = mapAssetFolderLoc + "\\" + modelName;
                    modelHolder = Model::create().withOBJ(filePath).withName(modelName).build();
                    // std::cout << "Asset folder: " << mapAssetFolderLoc << "\n";

                    // //create the world object map tile
                    // // WorldObject mapTile = WorldObject(meshHolder,materialsLoaded, positionHolder, rotationHolder, scaleHolder, isbuildableHolder, isPathHolder);

                    //create game object
                    auto mapTile = res->createGameObject(modelName);
                    // std::cout << "mesh Name: " << modelName << "\n";
                    auto mapTileMR = mapTile->addComponent<ModelRenderer>();
                    mapTile->addComponent<WorldObject>();
                    mapTile->getComponent<WorldObject>()->setBuildable(isbuildableHolder);
                    mapTile->getComponent<WorldObject>()->setIsPath(isPathHolder);

                    // bool test = mapTile->getComponent<WorldObject>()->getBuildableStatus();
                    // std::cout << "is buildable test: " << test  << "\n";
                    // OLD
//                    mapTileMR->setMesh(meshHolder);
//                    mapTileMR->setMaterials(materialsLoaded);
                    // NEW
                    mapTileMR->setModel(modelHolder);
                    float xOffset = d["MapLookup"][c]["posOffset"]["x"].GetFloat();
                    float yOffset = d["MapLookup"][c]["posOffset"]["y"].GetFloat();;
                    float zOffset = d["MapLookup"][c]["posOffset"]["z"].GetFloat();;

                    positionHolder.x += xOffset;
                    positionHolder.y += yOffset;
                    positionHolder.z += zOffset;

                    mapTile->getComponent<Transform>()->position = positionHolder;
                    mapTile->getComponent<Transform>()->rotation.y = rotationHolder;
                    // std::cout << "positionHolder.y:" << positionHolder.y << "\n";
                    // std::cout << "positionHolder.y:" << positionHolder.y << "\n";

                    mapTile->getComponent<Transform>()->scale = scaleHolder;
                    auto bounds = mapTileMR->getMesh()->getBoundsMinMax();

                    float length = (fabs(bounds[0].z) + fabs(bounds[1].z))/4;
                    float width = (fabs(bounds[0].x) + fabs(bounds[1].x))/4;
                    float height = (fabs(bounds[0].y) + fabs(bounds[1].y))/4;

                    // std::cout << "length: " << length << "\n";
                    // std::cout << "width: " << width << "\n";
                    // std::cout << "height: " << height << "\n";

                    // std::cout << "bounds: " << bounds[0].x << " " << bounds[0].y << bounds[0].z << "\n";


                    // mapTile->addComponent<RigidBody>()->initRigidBodyWithBox({0.5f,0.4f,0.5f}, 0);

                    mapTile->addComponent<RigidBody>()->initRigidBodyWithBox({length, height, width}, 0);
                    // mapTile->addComponent<RigidBody>()->initRigidBodyWithBox(bounds[0],0);
                    // worldTiles.push_back(mapTile); //Push the new map tile into the map tiles vector
                    // gameObjects.push_back(mapTile);

                    if (isPathHolder)
                    {
                        // pathHolder.push_back(positionHolder);
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
    // gameManager->setWaves(howManyWaves);
    // gameManager->waveAmount = howManyWaves;
    int enemyTypeInt;
    std::string enemyTypeStr;


    for (size_t wave = 0; wave < howManyWaves; wave++)
    { //wave level
        //get the number of enemyTypes and quantities in order
        int numberOfEnemySets = d["waves"][wave]["enemies"].GetArray().Size();
        std::vector<enemySetsInWave> enemySetsHolder;
        enemySetsInWave tempEnemySet{};

        //wave schedule parameters per this current wave
        waveScheduleDetails waveScheduleDetailHolder{};
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
            tempEnemySet.quantity = howManyOfEnemyType;
            enemySetsHolder.push_back(tempEnemySet);


            std::vector<glm::vec3> path = gameManager->getPath();
            int endOfPath = path.size();
            positionHolder = path[endOfPath-1]; //sets where the enemy will spawn
            positionHolder.y += 1.0; //compensates for the fact that the path is well, the ground
            // positionHolder = {3,3,3};

            scaleHolder.x = d["enemyLookup"][enemyTypeChar]["scaleFactors"]["x"].GetFloat();// scale
            scaleHolder.y = d["enemyLookup"][enemyTypeChar]["scaleFactors"]["y"].GetFloat();// scale
            scaleHolder.z = d["enemyLookup"][enemyTypeChar]["scaleFactors"]["z"].GetFloat();// scale


            modelName = d["enemyLookup"][enemyTypeChar]["object"].GetString();

            //Load the mesh from file
            // meshHolder = sre::ModelImporter::importObj(".\\Assets\\WorldMapAssets", "Floor01.obj", materialsLoaded);
            // OLD: meshHolder = sre::ModelImporter::importObj(mapAssetFolderLoc, modelName, materialsLoaded);
            // NEW
            auto filePath = enemiesAssetFolderLoc + "\\" + modelName;
            modelHolder = Model::create().withOBJ(filePath).withName(modelName).build();
            std::cout << "Asset folder: " << filePath << "\n";
            std::cout << "Model Name: " << modelName << "\n";

            // //create the world object map tile
            // // WorldObject mapTile = WorldObject(meshHolder,materialsLoaded, positionHolder, rotationHolder, scaleHolder, isbuildableHolder, isPathHolder);

            for (int anEnemy = 0; anEnemy < howManyOfEnemyType; anEnemy++)
            {
                // gameManager->enermyAmountWave += 1;
                //create game object
                auto enemy = res->createGameObject(modelName); // todo: incremental to tell skulls apart
                auto enemyTR = enemy->getComponent<Transform>();
                // std::cout << "mesh Name: " << modelName << "\n";
                auto enemyMR = enemy->addComponent<ModelRenderer>();
                auto enemyAN = enemy->addComponent<Animator>();
                enemyMR->setModel(modelHolder);
                enemyTR->setModelRenderer(enemyMR);
                enemyTR->setAnimator(enemyAN);

                auto idleAnimation = std::make_shared<Animation>(true);
                idleAnimation->addFrame(glm::vec3(0, 0.5, 0), glm::vec3(1), glm::vec3(0), .5f);
                idleAnimation->addFrame(glm::vec3(0, -0.5, 0), glm::vec3(1), glm::vec3(0), .5f);
                enemyAN->addAnimation("idle", idleAnimation);
                enemyAN->setAnimationState("idle");

                enemy->getComponent<Transform>()->position = positionHolder;
                enemy->getComponent<Transform>()->rotation.y = rotationHolder;
                enemy->getComponent<Transform>()->scale = scaleHolder;
                auto bounds = enemyMR->getMesh()->getBoundsMinMax();

                float length = (fabs(bounds[0].z) + fabs(bounds[1].z));
                float width = (fabs(bounds[0].x) + fabs(bounds[1].x));
                float height = (fabs(bounds[0].y) + fabs(bounds[1].y));

                length = (length * scaleHolder.z)*0.7; // scaling the collision box for a tighter fit
                // std::cout << "length: " << length << "\n";
                // std::cout << "width: " << width << "\n";
                // std::cout << "height: " << height << "\n";

                //Add Physics to skull
                enemy->addComponent<RigidBody>()->initRigidBodyWithSphere(length, 0); // mass of 0 sets the rigidbody as kinematic (or static)

                // std::cout << "anEnemy is: " << anEnemy << "\n";

                //Add Path Finder to Skull
                auto enemyEC = enemy->addComponent<EnemyComponent>();
                enemyEC->setEnemyNumber(anEnemy);
                enemyEC->setWave(wave);
                enemyEC->setEnemySetNumber(currentEnemySet);
                std::cout << "created enemy with enemy number: " << anEnemy << std::endl;
                std::cout << "created enemy with set number: " << currentEnemySet << std::endl;
                std::cout << "created enemy with wave number: " << wave << std::endl;
                enemy->addComponent<EnemyCollisionHandler>();
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

    //import model testing
    // std::vector<std::shared_ptr<sre::Material>> materials_unused;
    // mesh = sre::ModelImporter::importObj("./Floor_OBJ/", "Step01D.obj", materials_unused);
    // meshBanner = sre::ModelImporter::importObj("./Floor_OBJ/", "Floating_Banner.obj", materials_unused);
    // mat = sre::Shader::getStandardPBR()->createMaterial({{"S_SHADOW","true"}});
    // mat->setName("PBR material");
    // WorldObject test = WorldObject(mesh,mat, {0,-1.5,9},0,{1.0f,1.0f,1.0f});
    // WorldObject test2 = WorldObject(mesh,mat, {0,-1.5,5},0,{1.0f,1.0f,1.0f});
    // WorldObject test = WorldObject(mesh,materials_unused, {0,-1.5,9},0,{1.0f,1.0f,1.0f});
    // WorldObject test2 = WorldObject(mesh,materials_unused, {0,-1.5,5},0,{1.0f,1.0f,1.0f});

    // worldTiles.push_back(test);
    // worldTiles.push_back(test2);

    // std::cout << "ceilColor.x: " << ceilColor.x << "\n";
    // SoundEffectsPlayer mySpeaker;
    // SoundEffectsPlayer myOtherSpeaker;
    // // alSourcePlay(0);
    // mySpeaker.Play(soundA);
    // myOtherSpeaker.Play(1);
    // SourceManager * mySourceManager = SourceManager::Get(); // apparently worked!
    // mySourceManager->playSource((ALuint)1);
}

void SceneManager::changeScene(const std::shared_ptr<LevelData>& sceneData) {
    std::string path = ".\\maps\\";
    path.append(sceneData->fileName);

    if(sceneData->sceneType == 0)
    {
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
    else
    {
        auto scene = createMainMenuScene();
        setCurrentScene(scene);
        getCurrentScene()->guiManager = std::make_shared<MainMenuGuiManager>(gameManager);
        getCurrentScene()->gameManager = gameManager;
        getCurrentScene()->sceneManager = static_cast<const std::shared_ptr<SceneManager>>(this);
        gameManager->currentScene = getCurrentScene();
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

/*std::string path = ".\\maps\\";
    path.append(levelName);
    auto scene = createScene(path);
setCurrentScene(scene);
if(type == "level")
{
getCurrentScene()->guiManager = std::make_shared<LevelGuiManager>(gameManager);
}
else
getCurrentScene()->guiManager = std::make_shared<MainMenuGuiManager>(gameManager);
getCurrentScene()->gameManager = gameManager;
getCurrentScene()->sceneManager = static_cast<const std::shared_ptr<SceneManager>>(this);
gameManager->currentScene = getCurrentScene();
//loadMap(path, getCurrentScene());

auto scheduleManager = std::make_shared<ScheduleManager>();
scheduleManager->currentScene = getCurrentScene(); //not sure about this pattern, here the two managers 'know' each other
getCurrentScene()->scheduleManager = scheduleManager;

gameManager->setInitialWaveStats();
scheduleManager->fetchInitialWaveSchedule();*/

#pragma clang diagnostic pop