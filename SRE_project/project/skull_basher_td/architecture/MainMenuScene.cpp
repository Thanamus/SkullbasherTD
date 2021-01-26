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
#include "MainMenuScene.hpp"
#include "SceneManager.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


MainMenuScene::MainMenuScene(std::string name)
: Scene()
{
    this->name = name;
}

MainMenuScene::~MainMenuScene(){
}

void MainMenuScene::update(float deltaTime){
    for (auto& u : updatables){
        u->update(deltaTime);
    }
}

void MainMenuScene::onKey(SDL_Event &event){
    if (event.type != SDL_KEYDOWN)
        return;

    switch (event.key.keysym.sym)
    {
        case SDLK_p:
            //sceneManager.
            auto scene = sceneManager->createScene(".\\maps\\SkullBasherTDLevel0.json");
            sceneManager->setCurrentScene(scene);
            sceneManager->getCurrentScene()->guiManager = guiManager;
            sceneManager->getCurrentScene()->gameManager = gameManager;
            sceneManager->getCurrentScene()->sceneManager = sceneManager;
            gameManager->currentScene = sceneManager->getCurrentScene();
            sceneManager->loadMap(R"(.\maps\SkullBasherTDLevel0.json)", sceneManager->getCurrentScene());

            auto scheduleManager = std::make_shared<ScheduleManager>();
            scheduleManager->currentScene = sceneManager->getCurrentScene(); //not sure about this pattern, here the two managers 'know' each other
            sceneManager->getCurrentScene()->scheduleManager = scheduleManager;

            gameManager->setInitialWaveStats();
            scheduleManager->fetchInitialWaveSchedule();

            break;
    }
}

void MainMenuScene::onMouse(SDL_Event &event){

}

void MainMenuScene::render(){
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
        if (c->debugGui){
            guiManager->onGui();
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

#pragma clang diagnostic pop