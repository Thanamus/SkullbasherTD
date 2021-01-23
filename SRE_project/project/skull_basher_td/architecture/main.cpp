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


//sound Device
#include "SoundDevice.hpp" //i.e a device that is the "Listener"
#include "SoundEffectsLibrary.hpp" //i.e. SoundBuffer
#include "SoundEffectsPlayer.hpp" //i.e SoundSource or "Speaker" / object that has a voice
#include "MusicBuffer.hpp"

#include "SourceManager.hpp"

// #include "SoundNode.h"
// #include "Sound.h"

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

    // myNewSpeaker = mySpeaker;
    //setup sound
    SoundDevice * mySoundDevice = SoundDevice::Get();
    // mySoundDevice->SetAttunation(3);
    // uint32_t /*ALuint*/ sound1 = SoundEffectsLibrary::Get()->Load(".\\assets\\soundEffects\\spells\\pestilence.ogg");
    // soundA = SoundEffectsLibrary::Get()->Load(R"(.\assets\soundEffects\spells\pestilence.ogg)");
    // uint32_t soundB = SoundEffectsLibrary::Get()->Load(R"(.\assets\soundEffects\spells\pestilence.ogg)");
    // uint32_t soundC = SoundEffectsLibrary::Get()->Load(R"(.\assets\soundEffects\spells\pestilence.wav)");
    // uint32_t soundH = SoundEffectsLibrary::Get()->Load(R"(.\assets\soundEffects\spells\pestilence.wav)");
    // uint32_t soundD = SoundEffectsLibrary::Get()->Load(R"(.\assets\soundEffects\SoundPack1\Alarm.aif)");
    // uint32_t soundE = SoundEffectsLibrary::Get()->Load(R"(.\assets\soundEffects\NPC\gutteral_beast\mnstr1.wav)");
    // uint32_t soundF = SoundEffectsLibrary::Get()->Load("SRE_project\\project\\skull_basher_td\\assets\\soundEffects\\NPC\\gutteral_beast\\mnstr1.wav");
    // uint32_t soundG = SoundEffectsLibrary::Get()->Load("C:\\Users\\nfgol\\ITU_GProg\\SkullBasherTD\\SRE_project\\project\\skull_basher_td\\assets\\soundEffects\\NPC\\gutteral_beast\\mnstr2.wav");
    
    // \\project\\skull_basher_td\\Debug\\assets
    // SRE_project\project\skull_basher_td\assets\soundEffects\NPC\gutteral beast\mnstr1.wav

    SourceManager * mySourceManager = SourceManager::Get();
    // mySourceManager->playSource((ALuint)1);
        // SoundEffectsPlayer mySpeaker;
        // SoundEffectsPlayer myOtherSpeaker;
        // // alSourcePlay(0);
        // mySpeaker.Play(soundG);
        // myOtherSpeaker.Play(soundA);
    
    // Old way
    // MusicBuffer music(R"(.\assets\music\68-Gerudo_Valley.wav)");

    //new way - MusicBuffer is now a singleton
    MusicBuffer * myMusicBuffer = MusicBuffer::Get(); 
    myMusicBuffer->Load(R"(.\assets\music\68-Gerudo_Valley.wav)");
    // MusicBuffer music("..\\..\\project\\skull_basher_td\\assets\\Debug\\music\\The-Precipice-of-Victory-MP3.wav"); 

    // SoundNode musicThing;

    //handshaking
    gameManager = std::make_shared<GameManager>();
    gameManager->init();
    guiManager = std::make_shared<GuiManager>(gameManager);
    scheduleManager = std::make_shared<ScheduleManager>();
    guiManager->gameManager = gameManager;
    currentScene->guiManager = guiManager;
    currentScene->gameManager = gameManager;
    gameManager->currentScene = currentScene;

    //load map
    currentScene->loadMap(R"(.\maps\SkullBasherTDLevel0.json)", currentScene);

    scheduleManager->currentScene = currentScene; //not sure about this pattern, here the two managers 'know' each other
    currentScene->scheduleManager = scheduleManager;

    currentScene->gameManager->setInitialWaveStats();
    currentScene->scheduleManager->fetchInitialWaveSchedule();

    //Playing Sounds //TODO remove as these are tests


    myMusicBuffer->Play();

    r.frameUpdate = [&](float deltaTime){
        currentScene->update(deltaTime);

        mySourceManager->CheckAndReleaseOALSource();
        //Update Music buffer (pkeep playing music)
        myMusicBuffer->UpdateBufferStream();

        // if (mySpeaker.isPlaying() == false){
        //     mySpeaker.Play(soundA);

        // }
        // if (!music.isPlaying())
        // {
        //     std::cout << "Music is: " << music.isPlaying() << std::endl;
        //     // music.Play();
        //     // music.Stop();
        //     // music.Resume();
        // }
        
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



