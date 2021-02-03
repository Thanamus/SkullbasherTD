//#include <iostream>
//#include "sre/SDLRenderer.hpp"
//#include "scenes/Scene.hpp"
//#include "scenes/SceneManager.hpp"
//#include "GameObject.hpp"
//#include "Camera.hpp"
//#include "ModelRenderer.hpp"
//#include "Light.hpp"
//#include <string>
//#include <sre/Resource.hpp>
//
//#include "RigidBody.hpp"
//#include "CustomerCollisionHandler.hpp"
//
//#include "PersonController.hpp"
//#define GLM_ENABLE_EXPERIMENTAL
//#include "glm/gtx/string_cast.hpp"
//#include "BulletPhysics.hpp"
//#include "Main.hpp"
//#include "sre/Material.hpp"
//
//
////sound Device
//#include "SoundDevice.hpp" //i.e a device that is the "Listener"
//#include "SoundEffectsLibrary.hpp" //i.e. SoundBuffer
//#include "SoundEffectsPlayer.hpp" //i.e SoundSource or "Speaker" / object that has a voice
//#include "MusicBuffer.hpp"
//
//#include "SourceManager.hpp"
//#include "sol.hpp"
//#include "../LevelGuiManager.hpp"
//#include "../MainMenuGuiManager.hpp"
//
//// #include "SoundNode.h"
//// #include "Sound.h"
//
//Main::Main()
//{
//    using namespace sre;
//    SDLRenderer r;
//    r.init();
//
//    //setup sound
//
//    // myNewSpeaker = mySpeaker;
//    //--------- setup sound
//    SoundDevice * mySoundDevice = SoundDevice::Get(); // Initialise sound device
//    SourceManager * mySourceManager = SourceManager::Get(); // Initialise Source manager
//
//    //new way - MusicBuffer is now a singleton
//    MusicBuffer * myMusicBuffer = MusicBuffer::Get(); // Initialise music buffer
//    myMusicBuffer->Load(R"(.\assets\music\68-Gerudo_Valley.wav)"); // Start playing a music track. First music track played should use "Load()"
//
//    //-------------- handshaking
//    gameManager = std::make_shared<GameManager>();
//    gameManager->init();
//    guiManager = std::make_shared<MainMenuGuiManager>(gameManager);
//    //scheduleManager = std::make_shared<ScheduleManager>();
//    guiManager->gameManager = gameManager;
//
//    //gameManager->ToggleLockMouse();
//
//    sceneManager = std::make_shared<SceneManager>();
//    sceneManager->gameManager = gameManager;
//    guiManager->sceneManager = sceneManager;
//
//    //make scene
//    //auto scene = sceneManager->createScene(".\\maps\\SkullBasherTDLevel0.json");
//    auto scene = sceneManager->createMainMenuScene();
//    scene->sceneManager = sceneManager;
//    sceneManager->setCurrentScene(scene);
//
//    sceneManager->getCurrentScene()->guiManager = guiManager;
//    sceneManager->getCurrentScene()->gameManager = gameManager;
//    sceneManager->getCurrentScene()->sceneManager = sceneManager;
//    gameManager->currentScene = sceneManager->getCurrentScene();
//    //sceneManager->loadMap(R"(.\maps\SkullBasherTDLevel0.json)", sceneManager->getCurrentScene());
//
//    //scheduleManager->currentScene = sceneManager->getCurrentScene(); //not sure about this pattern, here the two managers 'know' each other
//    //sceneManager->getCurrentScene()->scheduleManager = scheduleManager;
//
//    //gameManager->setInitialWaveStats();
//    //scheduleManager->fetchInitialWaveSchedule();
//
////--------------- Start Playing music - lives here because the buffers run out before things load
//    myMusicBuffer->Play();
//
//// --------- start update cycles
//    r.frameUpdate = [&](float deltaTime){
//        sceneManager->getCurrentScene()->update(deltaTime);
//
//        mySourceManager->CheckAndReleaseOALSource(); // Checks for any sources that are finished playing their sound, and releases the source
//
//        //Update Music buffer (keep playing music - stream in the next few bytes of sound)
//        myMusicBuffer->UpdateBufferStream(); // Updates the music buffer so music keeps playing. Note: doesn't need to be called every frame, so we could optimise here
//    };
//    r.frameRender = [&]{
//        sceneManager->getCurrentScene()->render();
//    };
//    r.keyEvent = [&](SDL_Event &e) {
//        sceneManager->getCurrentScene()->onKey(e); //worked! // asks scene to manage the onKey
//    };
//    r.mouseEvent = [&](SDL_Event &e) {
//        sceneManager->getCurrentScene()->onMouse(e); // asks scene to manage the mouse thing
//    };
//    r.startEventLoop();
//}
//
//const sre::SDLRenderer &Main::getR() const {
//    return r;
//}
//
//int main(){
//    new Main();
//    return 0;
//}
//
//
//
#include <iostream>
#include <vector>
#include <fstream>

#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"
#include "sre/Material.hpp"
#include "sre/SDLRenderer.hpp"

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wformat-security"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sre/ModelImporter.hpp>
#include <glm/gtx/string_cast.hpp>
#include <sre/Inspector.hpp>
#include "Model.hpp"
#include "GameObject.hpp"
#include "ModelRenderer.hpp"
#include "Animator.hpp"
#include "Transform.hpp"

using namespace sre;

class ObjViewerExample {
public:
    ObjViewerExample(){
        r.init();

        SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
        r.otherEvent = [&](SDL_Event& e){
            if (e.type == SDL_DROPFILE){
                auto dropped_filedir = e.drop.file;
                loadObjFile(dropped_filedir);
                SDL_free(dropped_filedir);    // Free dropped_filedir memory
            }
        };

        camera.lookAt({0,0,3},{0,0,0},{0,1,0});
        camera.setPerspectiveProjection(60,0.1,farPlane);

//        models.push_back(std::make_pair(model, glm::vec3(0, 1, -0.4)));
        camera.lookAt({0,0,farPlane},{0,0,0},{0,1,0});
        camera.setPerspectiveProjection(60,0.1,farPlane);
//        model = Model::create().withOBJ(".\\assets\\models\\BarrelD.obj").withMaterials(materials).build();
        worldLights.addLight(sre::Light::create().build());
        worldLights.addLight(sre::Light::create().build());
        worldLights.addLight(sre::Light::create().build());
        worldLights.addLight(sre::Light::create().build());

        setLight(1);

        r.frameRender = [&](){
            render();
        };

        r.mouseEvent = [&](SDL_Event& event){
            if (event.button.button==SDL_BUTTON_RIGHT){
                showInspector = true;
            }
        };

        r.startEventLoop();
    }

    void setLight(int config){
        if (config == 0){
            worldLights.setAmbientLight({0.05f,0.05f,0.05f});
            worldLights.getLight(0)->lightType = LightType::Point;
            worldLights.getLight(0)->color = {1,1,1};
            worldLights.getLight(0)->position = {0,0,4};
            worldLights.getLight(0)->range = 100;
            worldLights.getLight(1)->lightType = LightType::Unused;
            worldLights.getLight(2)->lightType = LightType::Unused;
            worldLights.getLight(3)->lightType = LightType::Unused;
        }
        if (config == 1){
            worldLights.setAmbientLight({0.05f,0.05f,0.05f});
            worldLights.getLight(0)->lightType = LightType::Directional;
            worldLights.getLight(0)->color = {1,1,1};
            worldLights.getLight(0)->direction = {1,1,.2};
            worldLights.getLight(1)->lightType = LightType::Directional;
            worldLights.getLight(1)->color = {0,0,.3};
            worldLights.getLight(1)->direction = {-1,-1,-.8};
            worldLights.getLight(2)->lightType = LightType::Unused;
            worldLights.getLight(3)->lightType = LightType::Unused;
        }
        if (config == 2){
            worldLights.setAmbientLight({0.05f,0.05f,0.05f});
            worldLights.getLight(0)->lightType = LightType::Directional;
            worldLights.getLight(0)->color = {1,0,0};
            worldLights.getLight(0)->direction = {1,0,0};
            worldLights.getLight(1)->lightType = LightType::Directional;
            worldLights.getLight(1)->color = {0,1,0};
            worldLights.getLight(1)->direction = {-1,0,0};
            worldLights.getLight(2)->lightType = LightType::Unused;
            worldLights.getLight(3)->lightType = LightType::Unused;
        }
        if (config == 3){
            worldLights.setAmbientLight({0.05f,0.05f,0.05f});
            worldLights.getLight(0)->lightType = LightType::Point;
            worldLights.getLight(0)->color = {1,0,0};
            worldLights.getLight(0)->position = {0, 3,0};
            worldLights.getLight(0)->range = 20;
            worldLights.getLight(1)->lightType = LightType::Point;
            worldLights.getLight(1)->color = {0,1,0};
            worldLights.getLight(1)->position = {3, 0,0};
            worldLights.getLight(1)->range = 20;
            worldLights.getLight(2)->lightType = LightType::Point;
            worldLights.getLight(2)->color = {0,-3,0};
            worldLights.getLight(2)->position = {0,-3,0};
            worldLights.getLight(2)->range = 20;
            worldLights.getLight(3)->lightType = LightType::Point;
            worldLights.getLight(3)->color = {1,1,1};
            worldLights.getLight(3)->position = {-3,0,0};
            worldLights.getLight(3)->range = 20;
        }
    }

    void loadObjFile(std::string file){
        auto pos = file.find_last_of(kPathSeparator)+1;
        auto path = file.substr(0,pos);
        auto filename = file.substr(pos);
        materials.clear();
        auto model = Model::create().withOBJ(file).build();
        std::cout<<model->getMaterials().size()<< " "<< model->getMesh()->getIndexSets()<<std::endl;
        models.push_back(std::make_pair(model, glm::vec3{0, 0, 0}));
    }

    void render(){
        auto renderPass = RenderPass::create()
                .withCamera(camera)
                .withWorldLights(&worldLights)
                .withClearColor(true, {0, 0, 0, 1})
                .build();
        for(auto go : scene->getGameObjects()) {
            auto an  =   go->getComponent<Animator>();
            if(an)
                an->update(0.0167);
            go->getComponent<Transform>()->rotation = glm::vec3(0, (float)i, 0);
            go->getComponent<ModelRenderer>()->draw(&renderPass);
//            model.first->setTransform(glm::scale(glm::vec3(0.5f))*glm::eulerAngleY(glm::radians((float)i))*glm::translate(model.second));
//            renderPass.draw(model.first->getMesh(), model.first->getTransform(), model.first->getMaterials());
        }
        lightGUI();

        // align text
        drawTopText(
#ifdef EMSCRIPTEN
                "Drop obj not supported in Emscripten"
#else
                "Drop obj file here"
#endif
        );
        static Inspector inspector;
        inspector.update();
        if (showInspector){
            inspector.gui();
        }
        i++;
    }

    void updateLight(sre::Light* light,int i){
        ImGui::PushID(i);
        ImGui::LabelText("","Light index %i",i);
        const char* items[] = {"Point","Directional","None"};
        ImGui::Combo("Light type", (int*)(&light->lightType), items,3);
        if (light->lightType == LightType::Directional){
            ImGui::DragFloat3("Direction",&light->direction.x);
        }
        else if(light->lightType == LightType::Point){
            ImGui::DragFloat3("Position", &light->position.x);
            ImGui::DragFloat("Range", &light->range);
        }
        if (light->lightType != LightType::Unused){
            ImGui::ColorEdit3("Color", &light->color.x);
        }
        ImGui::PopID();
    }

    void lightGUI(){
        auto size = Renderer::instance->getWindowSize();
        ImVec2 imSize(250, 220.0f);
        ImVec2 imPos(size.x-250,50);
        ImGui::SetNextWindowSize(imSize);                                   // imgui window size should have same width as SDL window size
        ImGui::SetNextWindowPos(imPos);

        static bool lightOpen = false;
        ImGui::Begin("Light settings", &lightOpen);
        if (ImGui::CollapsingHeader("Predefined configs")){
            if (ImGui::Button("Camera light")){
                setLight(0);
            }
            if (ImGui::Button("Twin lights")){
                setLight(1);
            }
            if (ImGui::Button("Twin lights Left-Right")){
                setLight(2);
            }
            if (ImGui::Button("Colorful")){
                setLight(3);
            }
        }

        auto ambientLight = worldLights.getAmbientLight();
        if (ImGui::ColorEdit3("Ambient light",&ambientLight.x)){
            worldLights.setAmbientLight(ambientLight);
        }
        for (int i=0;i<4;i++){
            sre::Light* light = worldLights.getLight(i);
            updateLight(light,i);
        }
        ImGui::End();
    }

    void drawTopText(std::string txt){
        auto size = Renderer::instance->getWindowSize();
        ImVec2 imSize(size.x, 50.0f);
        ImVec2 imPos(0, 0);
        ImGui::SetNextWindowSize(imSize);                                   // imgui window size should have same width as SDL window size
        ImGui::SetNextWindowPos(imPos);
        ImGui::Begin("",nullptr,ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoTitleBar);
        // create window without title
        auto txtSize = ImGui::CalcTextSize(txt.c_str());                    // center text
        ImGui::SetCursorPosX((imSize.x-txtSize.x)/2);
        ImGui::TextWrapped(txt.c_str());
        ImGui::End();
    }
private:
    SDLRenderer r;
    sre::Camera camera;
    Scene* scene;
    WorldLights worldLights;
    std::shared_ptr<Mesh> mesh;
    std::vector<std::shared_ptr<Material>> materials;
    int i=0;
    glm::vec3 offset{0};
    float farPlane = 100;
    std::vector<std::pair<std::shared_ptr<Model>, glm::vec3>> models;
    const char kPathSeparator =
#ifdef _WIN32
            '\\';
#else
    '/';
#endif
    bool showInspector = false;
};

int main() {
    std::make_unique<ObjViewerExample>();
    return 0;
}

//#pragma clang diagnostic pop