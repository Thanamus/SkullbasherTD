//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Scene.hpp"
#include <algorithm>
#include <sre/Renderer.hpp>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "RigidBody.hpp"
#include "MeshRenderer.hpp"
#include "Light.hpp"
#include "BulletPhysics.hpp"
#include "sre/RenderPass.hpp"

//fps camera stuff
// #include "FirstPersonController.hpp"

//WorldMap Imports
    //WorldObject
#include "WorldObject.hpp"

//rapidjson imports
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <fstream>
#include <iostream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


Scene::Scene(std::string name)
:name(std::move(name))
{
    bulletPhysics = new BulletPhysics();
}

Scene::~Scene(){
    delete bulletPhysics;
	bulletPhysics = nullptr;
}

void Scene::update(float deltaTime){
    bulletPhysics->step(this);
    auto tempCam = this->cameras[0]->getGameObject();
    tempCam->getComponent<Camera>()->update(deltaTime);
    

    for (auto& p : this->rigidBodies){
        p->updateTransformFromPhysicsWorld();
    }
    for (auto& u : updatables){
        u->update(deltaTime);
    }
}

void Scene::render(){
    if (debugPhysics){
        bulletPhysics->debugDrawNewFrame();
    }
    worldLights.clear();
    worldLights.setAmbientLight(ambientColor);
    if (lights.size() > sre::Renderer::instance->getMaxSceneLights()){
        // instead for each rendered object closest lights and update wl
        std::cout << "Warn - too many scene lights" << std::endl;
    }
    for (auto & l : lights){
        worldLights.addLight(l->getLight());
    }

    for (auto c : cameras){
        c->bind();

        auto rp = sre::RenderPass::create()
                .withCamera(c->getCamera())
                .withClearColor(c->clearColorBuffer, c->clearColor)
                .withClearDepth(c->clearDepthBuffer)
                .withWorldLights(&worldLights)
                .withGUI(c->debugGui)
                .build();

        for (auto& comp : renderables){
            comp->draw(&rp);
        }
        if (debugPhysics){
            bulletPhysics->debugDraw(rp);
        }
        if (c->debugGui){
            ImGui::Begin(name.c_str());
            ImGui::Checkbox("Debug Physics", &debugPhysics);
            ImGui::ColorEdit3("Ambient light", &(ambientColor.x));
            if (ImGui::CollapsingHeader("GameObjects")){
                for (auto& go : gameObjects){
                    ImGui::PushID(go.get());
                    if (ImGui::CollapsingHeader(go->getName().c_str())){
                        for (auto& co : go->getComponents()) {
                            co->debugGUI();
                        }
                    }
                    ImGui::PopID();
                }
            }
            ImGui::End();
        }
    }
}

std::shared_ptr<GameObject> Scene::createGameObject(std::string name){
    auto res = std::shared_ptr<GameObject>(new GameObject(name, this));
    gameObjects.push_back(res);
    return res;
}

void Scene::addComponent(Component *component) {
    auto camera = dynamic_cast<Camera*>(component);
    if (camera){
        cameras.push_back(camera);
    }
    auto renderable = dynamic_cast<Renderable*>(component);
    if (renderable) {
        renderables.push_back(renderable);
    }
    auto updatable = dynamic_cast<Updatable*>(component);
    if (updatable) {
        updatables.push_back(updatable);
    }
    auto light = dynamic_cast<Light*>(component);
    if (light) {
        lights.push_back(light);
    }
    auto rigidBody = dynamic_cast<RigidBody*>(component);
    if (rigidBody) {
        rigidBodies.push_back(rigidBody);
    }

}

void Scene::removeComponent(Component *component) {
    auto camera = dynamic_cast<Camera*>(component);
    if (camera){
        cameras.erase(std::find(cameras.begin(),cameras.end(), camera));
    }
    auto renderable = dynamic_cast<Renderable*>(component);
    if (renderable) {
        renderables.erase(std::find(renderables.begin(), renderables.end(), renderable));
    }
    auto updatable = dynamic_cast<Updatable*>(component);
    if (updatable) {
        updatables.erase(std::find(updatables.begin(), updatables.end(), updatable));
    }
    auto light = dynamic_cast<Light*>(component);
    if (light) {
        lights.erase(std::find(lights.begin(), lights.end(), light));
    }
    auto rigidBody = dynamic_cast<RigidBody*>(component);
    if (rigidBody) {
        rigidBodies.erase(std::find(rigidBodies.begin(), rigidBodies.end(), rigidBody));
    }
}

bool Scene::isDebugPhysics() const {
    return debugPhysics;
}

void Scene::setDebugPhysics(bool debugPhysics) {
    Scene::debugPhysics = debugPhysics;
}

const glm::vec3 &Scene::getAmbientColor() const {
    return ambientColor;
}

void Scene::setAmbientColor(const glm::vec3 &ambientColor) {
    Scene::ambientColor = ambientColor;
}

const std::vector<std::shared_ptr<GameObject>> Scene::getGameObjects() {
    return gameObjects;
}


void Scene::loapMap(std::string filename, std::shared_ptr<Scene> res){

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

    int rowArrayCount = d["tileMap"].GetArray().Size(); //get the tile map key, the array size is the outset loop's size
    std::cout << "rowArrayCount: " << rowArrayCount << "\n";
    for (size_t row = 0; row < rowArrayCount; row++) //go through each 'row' of the map
    {
 
        mapRow.clear(); //clear the temporary buffer
        int heightArrayCount = d["tileMap"][row].GetArray().Size(); //inner array is the number of positions in the current row
        // std::cout << "inner array count:" << innerArrayCount << "\n";

        // push each field's value into the buffer
        for (size_t heightFactor = 0; heightFactor < heightArrayCount; heightFactor++){

            
            double tileHeight = tileHeightOffset + heightFactor;

            //variables defined here, attempting to save processing power
            int tileTypeInt;
            std::string tileTypeStr;
            float rotationHolder;
            glm::vec3 positionHolder;
            glm::vec3 scaleHolder;// scale

            bool isbuildableHolder = false;
            bool isPathHolder = false;
            std::shared_ptr<sre::Mesh> meshHolder;
            std::shared_ptr<sre::Material> matHolder;
            
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

                    std::string meshName = d["MapLookup"][c]["object"].GetString();

                    std::vector<std::shared_ptr<sre::Material>> materialsLoaded;
                    //Load the mesh from file
                    // meshHolder = sre::ModelImporter::importObj(".\\Assets\\WorldMapAssets", "Floor01.obj", materialsLoaded);
                    meshHolder = sre::ModelImporter::importObj(mapAssetFolderLoc, meshName, materialsLoaded);
                    // std::cout << "Asset folder: " << mapAssetFolderLoc << "\n";

                    // //create the world object map tile
                    // // WorldObject mapTile = WorldObject(meshHolder,materialsLoaded, positionHolder, rotationHolder, scaleHolder, isbuildableHolder, isPathHolder); 

                    //create game object
                    auto mapTile = res->createGameObject(meshName);
                    // std::cout << "mesh Name: " << meshName << "\n";
                    auto mapTileMR = mapTile->addComponent<MeshRenderer>();
                    mapTile->addComponent<WorldObject>();
                    mapTile->getComponent<WorldObject>()->setBuildable(isbuildableHolder);
                    mapTile->getComponent<WorldObject>()->setPath(isPathHolder);

                    bool test = mapTile->getComponent<WorldObject>()->getBuildableStatus();
                    // std::cout << "is buildable test: " << test  << "\n";

                    mapTileMR->setMesh(meshHolder);
                    mapTileMR->setMaterial(materialsLoaded[0]);

                    float xOffset = d["MapLookup"][c]["posOffset"]["x"].GetFloat();
                    float yOffset = d["MapLookup"][c]["posOffset"]["y"].GetFloat();;
                    float zOffset = d["MapLookup"][c]["posOffset"]["z"].GetFloat();;

                    positionHolder.x += xOffset;
                    positionHolder.y += yOffset;
                    positionHolder.z += zOffset;

                    mapTile->getComponent<Transform>()->position = positionHolder;
                    // std::cout << "positionHolder.y:" << positionHolder.y << "\n";
                    // std::cout << "positionHolder.y:" << positionHolder.y << "\n";

                    mapTile->getComponent<Transform>()->scale = scaleHolder;
                    auto bounds = meshHolder->getBoundsMinMax();
                    
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


                }
            }
            
        }
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

}

// }


#pragma clang diagnostic pop