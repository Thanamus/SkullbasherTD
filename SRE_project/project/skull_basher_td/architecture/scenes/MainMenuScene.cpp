//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//
#include <sre/Renderer.hpp>
#include "../CameraComponent.hpp"
#include "../physics/RigidBodyComponent.hpp"
#include "../LightComponent.hpp"
#include "../physics/BulletPhysics.hpp"

//fps camera stuff
#include "../PersonControllerComponent.hpp"

//WorldMap Imports
//WorldObjectComponent
#include "../WorldObjectComponent.hpp"
#include "../sound/SoundEffectsLibrary.hpp"

//rapidjson imports
#include <iostream>
#include "MainMenuScene.hpp"

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