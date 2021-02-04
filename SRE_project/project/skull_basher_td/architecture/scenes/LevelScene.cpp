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
#include "../ModelRenderer.hpp"

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
    for (auto& p : this->rigidBodies){
        p->updateTransformFromPhysicsWorld();
    }
    // bulletPhysics->step(this);
    scheduleManager->update(deltaTime); //has to be updated separately from the rest

    // delete dead objects
    // for (int i=0;i<gameObjects.size();i++){
    //     // gameObjects[i]->update(time);

    //     if (gameObjects[i]->deleteMe == true) // looks for deleteMe flag on the game object, if true, then remove the gameObject
    //     {
    //             auto components = gameObjects[i]->getComponents();
    //             for (auto c : components)
    //             {
    //                 gameObjects[i]->removeComponent(c);
    //                 // this->removeComponent<Renderable>(c);
    //             }
    //         gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObjects[i]), gameObjects.end()); // maintains the sceneObjects list
    //     }

    // }


    for (auto g : gameObjects)
    {
        if (g->deleteMe == true) // looks for deleteMe flag on the game object, if true, then remove the gameObject
        {
            // // TODO find and cleanup dangling shared_ptrs
            // // std::cout << "g count 1: " << g.use_count() << std::endl;
            // // auto camera = gameObjects[i]->getComponent<Camera>();
            // // TODO check for updateable and renderable?
            // auto thing =  g->getComponent<RigidBody>();



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

void LevelScene::SpawnCoin(glm::vec3 position) {
    auto coin = this->createGameObject("Coin");
    coin->getComponent<Transform>()->position = position;
    coin->getComponent<Transform>()->rotation = {0,0,0};
    coin->getComponent<Transform>()->scale = {0.4f,0.4f,0.4f};
    auto coinMR = coin->addComponent<ModelRenderer>();
    auto coinPath =  ".\\assets\\Coins.obj";

    //TODO: review animation
    auto coinAN = coin->addComponent<Animator>();
    coinMR->setAnimator(coinAN.get());
    auto coinRotate = std::make_shared<Animation>(true);
    coinRotate->addFrame(glm::vec3( 0), glm::vec3(1), glm::vec3(1), 5.f);
    coinRotate->addFrame(glm::vec3( 0), glm::vec3(1), glm::vec3(359), 5.f);
    coinAN->addAnimation("rotate", coinRotate);
    coinAN->setAnimationState("rotate");
    coinMR->setModel(Model::create().withOBJ(coinPath).withName("coin").build());
    auto bounds = coinMR->getMesh()->getBoundsMinMax();

    float length = (fabs(bounds[0].z) + fabs(bounds[1].z));
    float width = (fabs(bounds[0].x) + fabs(bounds[1].x))/5;
    float height = (fabs(bounds[0].y) + fabs(bounds[1].y))/12;

    auto coinRigidBody = coin->addComponent<RigidBody>();
    coinRigidBody->initRigidBodyWithBox({length,width,height}, 1, COINS, PLAYER ); // crystal needs to be sphere -> skull collision only works with box

    // ---- making sure that Coin can't move if hit
    coinRigidBody->getRigidBody()->setAngularFactor(btVector3(0,0,0));
    coinRigidBody->getRigidBody()->setLinearFactor(btVector3(0,0,0));
}

#pragma clang diagnostic pop