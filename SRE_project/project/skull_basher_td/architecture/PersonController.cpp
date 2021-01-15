//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include "PersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

//using namespace sre;
using namespace glm;

// #include "SoundDevice.hpp"
#include "Component.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"
// #include "Scene.hpp"


PersonController::PersonController(GameObject* gameObject) 
: Component(gameObject)
{   
    camera = gameObject->getComponent<Camera>();
    // camera->setPerspectiveProjection(45, 0.1f, 1000);
    // position = vec3(0, 0, 0);
    camera_front = gameObject->getComponent<Transform>()->rotation;                         // set initial target of the camera on the negative Z axis (default)
    // camera_front = vec3(0, 0, -1);                         // set initial target of the camera on the negative Z axis (default)
    camera_dir = normalize(position - camera_front);       // sets the camera direction with a positive Z axis
    camera_right = normalize(cross(world_up, camera_dir)); // get a right vector perpendicular to the Y axis and the Z axis

    //initialise main sound device to reflect the PersonController
    // m_earsToListenWith->SetLocation(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z));
    // m_earsToListenWith->SetOrientation(camera_dir.x, camera_dir.y, camera_dir.z, 
    // world_up.x, world_up.y, world_up.z);

}
// FirstPersonController::FirstPersonController(sre::Camera *camera)
//     : camera(camera)
// {
//     camera->setPerspectiveProjection(45, 0.1f, 1000);
//     position = vec3(0, 0, 0);
//     camera_front = vec3(0, 0, -1);                         // set initial target of the camera on the negative Z axis (default)
//     camera_dir = normalize(position - camera_front);       // sets the camera direction with a positive Z axis
//     camera_right = normalize(cross(world_up, camera_dir)); // get a right vector perpendicular to the Y axis and the Z axis
// }

void PersonController::update(float deltaTime)
{
    updateInput(deltaTime);
    updateVectors();

    camera_front = glm::normalize(vec3(cos(radians(rotation)), 0, sin(radians(rotation)))); // update camera "target" to match rotation
    camera->TestRay(camera_front, this->rayTestedCube);

    // camera->lookAt(position, position + camera_front, world_up);
    this->getGameObject()->getComponent<Transform>()->lookAt(position + camera_front, world_up);

    //update listener position and orientation
    // m_earsToListenWith->SetLocation(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z));
    // m_earsToListenWith->SetOrientation(camera_dir.x, camera_dir.y, camera_dir.z, 
    //     world_up.x, world_up.y, world_up.z);

    // glm::vec3 thing(0.f,0.f,0.f);
    // gameObject->getScene()->listener->GetLocation(thing.x, thing.y, thing.z);
    // std::cout << "thing.x" << thing.x << std::endl;
}

void PersonController::updateVectors()
{
    camera_dir = normalize(position - camera_front); // sets the camera direction with a positive Z axis
    camera_right = normalize(cross(world_up, camera_dir));
}

void PersonController::updateInput(float deltaTime)
{
    float velocity = movespeed * deltaTime;

    auto thing = this->getGameObject()->getComponent<Transform>();
    // thing->rotation = 
    // update rotation
    rotation += mouse_offset;
    // keep rotation in 360 degree range
    if (rotation > 360.f)
        rotation -= 360.f;
    else if (rotation < -0.f)
        rotation += 360.f;
    mouse_offset = 0; // reset offset

    // update position based on current keypresses
    // using the camerafront vector allows to keep account of rotation automatically
    position = thing->position;

    if (key_fwd)
        position += velocity * camera_front;
    if (key_bwd)
        position -= velocity * camera_front;
    if (key_left)
        position -= velocity * cross(camera_front, world_up);
    if (key_right)
        position += velocity * cross(camera_front, world_up);
    if (key_flyUp)
        position += velocity * world_up; //works! fly's up
    if (key_flyDown)
        position -= velocity * world_up; //flys down!
    
    thing->position = position;
}

void PersonController::onKey(SDL_Event &event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_w:
        key_fwd = event.type == SDL_KEYDOWN;
        // std::cout << "forward \n";
        break;
    case SDLK_a:
        key_left = event.type == SDL_KEYDOWN;
        break;
    case SDLK_s:
        key_bwd = event.type == SDL_KEYDOWN;
        break;
    case SDLK_d:
        key_right = event.type == SDL_KEYDOWN;
        break;
    case SDLK_r:
        key_flyUp = event.type == SDL_KEYDOWN;
        break;
    case SDLK_f:
        key_flyDown = event.type == SDL_KEYDOWN;
        break;                
    case SDLK_ESCAPE:
        quit = event.type == SDL_KEYDOWN;
        break;
    default:
        break;
    }
}

void PersonController::onMouse(SDL_Event &event)
{
    if (event.type == SDL_MOUSEMOTION)
    {
        // std::cout << "mouse event recorded \n";
        mouse_offset = event.motion.xrel * sensitivity;
    }
}

void PersonController::setInitialPosition(glm::vec2 position, float rotation)
{
    this->position = glm::vec3(position.x, 0, position.y);
    this->rotation = rotation;
}



// class MyClass
// {
// private:
//     // MyClass owns the unique_ptr.
//     unique_ptr<ClassFactory> factory;
// public:

//     // Initialize by using make_unique with ClassFactory default constructor.
//     MyClass() : factory (make_unique<ClassFactory>())
//     {
//     }

//     void MakeClass()
//     {
//         factory->DoSomething();
//     }
// };