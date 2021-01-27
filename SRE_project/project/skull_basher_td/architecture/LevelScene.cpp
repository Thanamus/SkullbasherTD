//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include <algorithm>
#include <sre/Renderer.hpp>
#include "Camera.hpp"
#include "GameObject.hpp"
#include "RigidBody.hpp"
#include "ModelRenderer.hpp"
#include "Light.hpp"
#include "BulletPhysics.hpp"
#include "sre/RenderPass.hpp"
#include "../GameManager.hpp"

//fps camera stuff
#include "PersonController.hpp"

//WorldMap Imports
    //WorldObject
#include "WorldObject.hpp"

#include "PathFinder.hpp"

#include <AL/al.h>
#include "SoundEffectsPlayer.hpp"
#include "SoundEffectsLibrary.hpp"
#include "SourceManager.hpp"

//rapidjson imports
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <fstream>
#include <iostream>
#include "LevelScene.hpp"

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
    bulletPhysics->step(this);
    auto tempCam = this->cameras[0]->getGameObject();
    tempCam->getComponent<PersonController>()->update(deltaTime); // TODO could probably remove this by making PersonController inherit from Updateable
    

    for (auto& p : this->rigidBodies){
        p->updateTransformFromPhysicsWorld();
    }
    for (auto& u : updatables){
        u->update(deltaTime);
    }
    scheduleManager->update(deltaTime); //has to be updated separately from the rest
}

void LevelScene::onKey(SDL_Event &event){
    auto tempCam = this->cameras[0]->getGameObject(); // gets the main camera object and gets the game object from that
    tempCam->getComponent<PersonController>()->onKey(event); //camera game object has a PersonController
    gameManager->onKey(event);
}

void LevelScene::onMouse(SDL_Event &event){
    auto tempCam = this->cameras[0]->getGameObject(); //gets the main camera
    tempCam->getComponent<PersonController>()->onMouse(event); //triggers the onMouse event handling in the Person controller
    gameManager->onMouse(event);
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