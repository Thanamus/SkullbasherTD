//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include <sre/Renderer.hpp>
#include "../Camera.hpp"
#include "../physics/RigidBody.hpp"
#include "../Light.hpp"
#include "../physics/BulletPhysics.hpp"

//fps camera stuff
#include "../PersonController.hpp"

//WorldMap Imports
    //WorldObject
#include "../WorldObject.hpp"

#include "../sound/SoundEffectsLibrary.hpp"

//rapidjson imports
#include "../rapidjson/rapidjson.h"

#include <iostream>
#include "LevelScene.hpp"
#include "../../GameManager.hpp"

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
    tempCam->getComponent<PersonController>()->update(deltaTime); // TODO could probably remove this by making PersonController inherit from Updateable

    bulletPhysics->step(this);
    for (auto& u : updatables){
        u->update(deltaTime);
    }
    // bulletPhysics->step(this);
    for (auto& p : rigidBodies){
        p->updateTransformFromPhysicsWorld();
    }
    for (auto& s : scriptables){
        if(s->isEnabled())
            s->update();
    }

    bulletPhysics->step(this);
    scheduleManager->update(deltaTime); //has to be updated separately from the rest

    // delete dead objects
    for (auto g : gameObjects)
    {


            if (g->deleteMe == true) // looks for deleteMe flag on the game object, if true, then remove the gameObject
            {
                // TODO find and cleanup dangling shared_ptrs
                // std::cout << "g count 1: " << g.use_count() << std::endl;
                // auto camera = gameObjects[i]->getComponent<Camera>();
                auto components = g->getComponents();


                for (auto c : components)
                {

                    g->removeComponent(c);
                    // this->removeComponent<Renderable>(c);

                }

                // std::make_unique<GameObject>(g);
                // std::cout << "g count 2: " << g.use_count() << std::endl;
                g.reset();


                gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), g), gameObjects.end()); // maintains the sceneObjects list
                // std::cout << "game Object destroyed";
                // TODO: something not quite right here
                // std::cout << "g count 3: " << g.use_count() << std::endl;
            }



    }

}

void LevelScene::onKey(SDL_Event &event){
    auto tempCam = this->cameras[0]->getGameObject(); // gets the main camera object and gets the game object from that
    tempCam->getComponent<PersonController>()->onKey(event); //camera game object has a PersonController
    GameManager::getInstance().onKey(event);
}

void LevelScene::onMouse(SDL_Event &event){
    auto tempCam = this->cameras[0]->getGameObject(); //gets the main camera
    tempCam->getComponent<PersonController>()->onMouse(event); //triggers the onMouse event handling in the Person controller
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