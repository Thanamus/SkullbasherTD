#include <iostream>
#include "sre/SDLRenderer.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "ModelRenderer.hpp"
#include "Light.hpp"
#include "RigidBody.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "BulletPhysics.hpp"

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

std::shared_ptr<Scene> createScene(){
    auto res = std::make_shared<Scene>();
    auto cameraObj = res->createGameObject("Camera");
    cameraObj->addComponent<Camera>()->clearColor = {0.2,0.2,0.2};
    cameraObj->getComponent<Transform>()->position = {10,1.7f,-5};
    cameraObj->getComponent<Transform>()->rotation = {0,190,0};
    

    
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
    res->physicsCube = cube;

    auto rayTestCube = res->createGameObject("Ray Tested Cube");
    rayTestCube->getComponent<Transform>()->position = {0,0,0};
    rayTestCube->getComponent<Transform>()->rotation = {0,0,0};
    auto rayTestCubeMR = rayTestCube->addComponent<ModelRenderer>();
    rayTestCubeMR->setMesh(sre::Mesh::create().withCube(0.99).build());
    res->rayTestedCube = rayTestCube;
    //Load Map
    res->loadMap(".\\maps\\SkullBasherTDLevel0.json", res);
    // res->loadMap("level0.json",res);


    return res;
}

int main(){
    // std::cout << "Hello world" << "\n";
    using namespace sre;
    SDLRenderer r;
    r.init();
    auto scene = createScene();

    r.frameUpdate = [&](float deltaTime){
        scene->update(deltaTime);
    };
    r.frameRender = [&]{
        scene->render();
    };
    r.mouseEvent = [&](SDL_Event &e) {
        float x = e.motion.x;
        float y = e.motion.y;
        glm::vec2 mousePos;
        mousePos.x = x;
        mousePos.y = y;
        scene->cameras[0]->TestRay(scene->rayTestedCube, mousePos);
        /*glm::vec3 startPos = scene->cameras[0]->getCamera().getPosition();
        glm::vec3 endPos = startPos;
        endPos.z += 15.0f;

        scene->physicsCube->getComponent<BulletPhysics>()->RaycastWorld(startPos,endPos);*/
    };

    r.startEventLoop();

    return 0;
}
