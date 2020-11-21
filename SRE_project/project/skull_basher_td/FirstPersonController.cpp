//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include "FirstPersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

using namespace sre;
using namespace glm;

FirstPersonController::FirstPersonController(sre::Camera *camera)
    : camera(camera)
{
    camera->setPerspectiveProjection(45, 0.1f, 1000);
    position = vec3(0, 0, 0);
    camera_front = vec3(0, 0, -1);                         // set initial target of the camera on the negative Z axis (default)
    camera_dir = normalize(position - camera_front);       // sets the camera direction with a positive Z axis
    camera_right = normalize(cross(world_up, camera_dir)); // get a right vector perpendicular to the Y axis and the Z axis
}

void FirstPersonController::update(float deltaTime)
{
    updateInput(deltaTime);
    updateVectors();

    camera_front = glm::normalize(vec3(cos(radians(rotation)), 0, sin(radians(rotation)))); // update camera "target" to match rotation

    camera->lookAt(position, position + camera_front, world_up);
}

void FirstPersonController::updateVectors()
{
    camera_dir = normalize(position - camera_front); // sets the camera direction with a positive Z axis
    camera_right = normalize(cross(world_up, camera_dir));
}

void FirstPersonController::updateInput(float deltaTime)
{
    float velocity = movespeed * deltaTime;

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
    if (key_fwd)
        position += velocity * camera_front;
    if (key_bwd)
        position -= velocity * camera_front;
    if (key_left)
        position -= velocity * cross(camera_front, world_up);
    if (key_right)
        position += velocity * cross(camera_front, world_up);
}

void FirstPersonController::onKey(SDL_Event &event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_w:
        key_fwd = event.type == SDL_KEYDOWN;
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
    case SDLK_ESCAPE:
        quit = event.type == SDL_KEYDOWN;
        break;
    default:
        break;
    }
}

void FirstPersonController::onMouse(SDL_Event &event)
{
    if (event.type == SDL_MOUSEMOTION)
    {
        mouse_offset = event.motion.xrel * sensitivity;
    }
}

void FirstPersonController::setInitialPosition(glm::vec2 position, float rotation)
{
    this->position = glm::vec3(position.x, 0, position.y);
    this->rotation = rotation;
}
