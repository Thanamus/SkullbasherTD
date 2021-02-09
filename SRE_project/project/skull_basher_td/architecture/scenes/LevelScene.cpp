//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include <sre/Renderer.hpp>
#include "../CameraComponent.hpp"
#include "../physics/RigidBody.hpp"
#include "../LightComponent.hpp"
#include "../physics/BulletPhysics.hpp"

//fps camera stuff
#include "../PersonControllerComponent.hpp"

//WorldMap Imports
    //WorldObjectComponent
#include "../WorldObjectComponent.hpp"

#include "../sound/SoundEffectsLibrary.hpp"

//rapidjson imports
#include "../rapidjson/rapidjson.h"

#include <iostream>
#include "LevelScene.hpp"
#include "../../GameManager.hpp"
#include "../ModelRendererComponent.hpp"
#include "../CoinComponent.hpp"
#include "../collisions/CoinCollisionHandler.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


LevelScene::LevelScene(std::string name)
: Scene()
{
    this->name = name;
    bulletPhysics = new BulletPhysics();
}

LevelScene::~LevelScene(){
    delete bulletPhysics;
	bulletPhysics = nullptr;
}

void LevelScene::update(float deltaTime){
    if(GameManager::getInstance().paused || !GameManager::getInstance().levelRunning)
        return;
    bulletPhysics->step(this);
    auto tempCam = this->cameras[0]->getGameObject();
    tempCam->getComponent<PersonControllerComponent>()->update(deltaTime); // TODO could probably remove this by making PersonControllerComponent inherit from Updateable
    bulletPhysics->step(this);
    for (const auto& g : gameObjects)
        if (g && g->isQueuedForDeletion())// looks for queuedForDeletion flag on the game object, if true, then remove the gameObject
            deleteGameObject(g);
    for (auto& u : updatables)
        u->update(deltaTime);
    for (auto& p : rigidBodies)
        p->updateTransformFromPhysicsWorld();
    for (auto& s : scriptables)
        if(s->isEnabled())
            s->update(deltaTime);
    scheduleManager->update(deltaTime); //has to be updated separately from the rest
}

void LevelScene::onKey(SDL_Event &event){
    auto tempCam = this->cameras[0]->getGameObject(); // gets the main camera object and gets the game object from that
    tempCam->getComponent<PersonControllerComponent>()->onKey(event); //camera game object has a PersonControllerComponent
    GameManager::getInstance().onKey(event);
}

void LevelScene::onMouse(SDL_Event &event){
    auto tempCam = this->cameras[0]->getGameObject(); //gets the main camera
    tempCam->getComponent<PersonControllerComponent>()->onMouse(event); //triggers the onMouse event handling in the Person controller
    GameManager::getInstance().onMouse(event);
}

void LevelScene::render(){
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

    for (auto & c : cameras){
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
            guiManager->onGui();

            static sre::Inspector inspector;
            inspector.update();
            if(debugPerformance)
                inspector.gui();

            ImGui::Begin(name.c_str());
            ImGui::Checkbox("Debug Physics", &debugPhysics);
            ImGui::Checkbox("Debug Performance", &debugPerformance);
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

#pragma clang diagnostic pop