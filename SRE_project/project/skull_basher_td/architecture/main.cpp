#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "ModelRenderer.hpp"
#include "Light.hpp"
#include "RigidBody.hpp"
#include "PersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "BulletPhysics.hpp"
#include "Main.hpp"
#include "sre/Material.hpp"

Main::Main()
{
    // std::cout << "Hello world" << "\n";
    using namespace sre;
    SDLRenderer r;
    r.init();
    //make scence
    auto scene = createScene();
    currentScene = scene;

    auto tempCam = currentScene->cameras[0]->getGameObject();
    // tempCam->getComponent<Camera>()->update(deltaTime);
    tempCam->getComponent<PersonController>()->currentScene = currentScene;

    //handshaking
    gameManager = std::make_shared<GameManager>();
    gameManager->init();
    guiManager = std::make_shared<GuiManager>(gameManager);
    scheduleManager = std::make_shared<ScheduleManager>();
    guiManager->gameManager = gameManager;
    currentScene->guiManager = guiManager;
    currentScene->gameManager = gameManager;
    gameManager->currentScene = currentScene;
    gameManager->ToggleLockMouse();

    //load map
    currentScene->loadMap(".\\maps\\SkullBasherTDLevel0.json", currentScene);

    scheduleManager->currentScene = currentScene; //not sure about this pattern, here the two managers 'know' each other
    currentScene->scheduleManager = scheduleManager;

    currentScene->gameManager->setInitialWaveStats();
    currentScene->scheduleManager->fetchInitialWaveSchedule();

    r.frameUpdate = [&](float deltaTime){
        currentScene->update(deltaTime);
    };
    r.frameRender = [&]{
        currentScene->render();
    };
    r.keyEvent = [&](SDL_Event &e) {
        currentScene->onKey(e); //worked! // asks scene to manage the onKey
        // fpsController.onKey(e);
        // gameManager->onKey(e);
    };
    r.mouseEvent = [&](SDL_Event &e) {
        // if (!lockRotation) //carry over from Wolf3D... not sure what it's there for
        // {
        currentScene->onMouse(e); // asks scene to manage the mouse thing
        // }
    };
    r.startEventLoop();
}
#include "Animator.hpp"

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

std::shared_ptr<Scene> Main::createScene(){
    auto res = std::make_shared<Scene>();
    auto cameraObj = res->createGameObject("Camera");
    cameraObj->addComponent<Camera>()->clearColor = {0.2,0.2,0.2};
    cameraObj->getComponent<Transform>()->position = {20,1.0f,11};
    cameraObj->getComponent<Transform>()->rotation = {0,190,0};
    cameraObj->addComponent<PersonController>(); // adding the controller to the camera (the player)
    // cameraObj->addComponent<RigidBody>()->initRigidBodyWithSphere(0.1f, 0);
    // cameraObj->addComponent<RigidBody>();
    

    
    auto sphereObj = res->createGameObject("Sphere");
    auto sphereMR = sphereObj->addComponent<ModelRenderer>();
    sphereMR->setMesh(sre::Mesh::create().withSphere(16,32,0.99f).build());
    sphereObj->getComponent<Transform>()->position = {10,1.0,10};
    sphereObj->getComponent<Transform>()->position = {10,1.0,10};
    sphereObj->addComponent<RigidBody>()->initRigidBodyWithSphere(1, 0);

    // auto planeObj = res->createGameObject("Plane");
    // auto plameMR = planeObj->addComponent<ModelRenderer>();
    // plameMR->setMesh(sre::Mesh::create().withQuad(10).build());
    // planeObj->getComponent<Transform>()->rotation = {-90,0,0};
    // auto planePhysObj = res->createGameObject("PlanePhys");
    // planePhysObj->addComponent<RigidBody>()->initRigidBodyWithStaticPlane({0,1,0}, 0);

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
    tower->getComponent<Transform>()->scale = {0.5f,0.5f,0.5f};
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

    //Load Map
    // res->loadMap(".\\maps\\SkullBasherTDLevel0.json", res);
    // res->loadMap("level0.json",res);

    return res;
}

const sre::SDLRenderer &Main::getR() const {
    return r;
}

int main(){
    new Main();
    return 0;
}
