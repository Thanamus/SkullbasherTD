//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include "PersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#include "SoundDevice.hpp"

// btKinematicCharacterController includes
#include "btKinematicCharacterController.h"
// #include "btGhostObject.h" // Aparrently doesn't exist
#include "RigidBody.hpp"

//using namespace sre;
using namespace glm;

PersonController::PersonController(GameObject* gameObject)
 : Component(gameObject)
{
    camera = gameObject->getComponent<Camera>();
    btRigidBody* myBody = gameObject->getComponent<RigidBody>()->getRigidBody();
    myBody->setAngularFactor({0,0,0});
    myBody->setActivationState(DISABLE_DEACTIVATION);
    // camera->setPerspectiveProjection(45, 0.1f, 1000);
    // position = vec3(0, 0, 0);
    camera_front = gameObject->getComponent<Transform>()->rotation;  // set initial target of the camera on the negative Z axis (default)
    
    // glm::vec3 glmCameraPosition =  cameraObj->getComponent<Transform>()->position;
    // btTransform transform;
    // btVector3 btCameraPosition = {glmCameraPosition.x, glmCameraPosition.y, glmCameraPosition.z}; 
    // transform.setOrigin(btCameraPosition);

    // glm::quat myquaternion = glm::quat(glm::vec3(angle.x, angle.y, angle.z));

    // glm::quat inputQuat = glm::quat(cameraObj->getComponent<Transform>()->rotation);
    // glm::quat inputQuat = glm::quat(camera_front);
    // btQuaternion btInputQuat = {inputQuat.w, inputQuat.x, -inputQuat.y,inputQuat.z }; 
    // transform.setRotation(-btInputQuat);

    // myBody->setWorldTransform(transform);
    // cameraObj->addComponent<RigidBody>();
    
    // btVector3 camera_front_bt = {camera_front.x, camera_front.y, camera_front.z};
    // myBody->getOrientation().setRotation({0,1,0}, radians(rotation));
    // btQuaternion cameraOrientation = myBody->getOrientation();
    // btVector3  camera_front_bt = cameraOrientation.setRotation();
    // camera_front = vec3(0, 0, -1);                         // set initial target of the camera on the negative Z axis (default)
    camera_dir = normalize(position - camera_front);       // sets the camera direction with a positive Z axis
    camera_right = normalize(cross(world_up, camera_dir)); // get a right vector perpendicular to the Y axis and the Z axis



// -- Trying to add btKinematicCharacterController
    // btGhostObject ghostObject = btGhostObject();
    // btPairCachingGhostObject * thing = ghostObject;
    // btKinematicCharacterController kinematicPlayerController = btKinematicCharacterController(ghostObject, btSphereShape);

//     btKinematicCharacterController::btKinematicCharacterController	(	btPairCachingGhostObject * 	ghostObject,
// btConvexShape * 	convexShape,
// btScalar 	stepHeight,
// const btVector3 & 	up = btVector3(1.0,0.0,0.0) 
// )	

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

void PersonController::debugGUI() {
    ImGui::PushID(this);
    if (ImGui::TreeNode("PersonController")) {
        ImGui::Checkbox("AllowedToBuild", &allowedToBuild);
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void PersonController::update(float deltaTime)
{
    updateVectors();
    updateInput(deltaTime);

    camera_front = glm::normalize(vec3(cos(radians(rotation)), 0, sin(radians(rotation)))); // update camera "target" to match rotation
    camera->moveHandCursor(camera_front, this->hand);
    if(currentScene->gameManager->buildModeActive)
    {
        camera->moveTowerCursor(camera_front, this->tower);
        camera->simpleRayCast(camera_front, this->tower, currentScene->getGameObjects());
    }


    // camera->lookAt(position, position + camera_front, world_up);
    this->getGameObject()->getComponent<Transform>()->lookAt(position + camera_front, world_up);

    // update Listener
    // TODO make this into a helper function instead
    SoundDevice::Get()->SetLocation(position.x, position.y, position.z);
    SoundDevice::Get()->SetOrientation(camera_front.x, camera_front.y, camera_front.z,
                                        world_up.x, world_up.y, world_up.z);
    // glm::vec3 testingPos;
    // SoundDevice::Get()->GetLocation(testingPos.x, testingPos.y, testingPos.z);
    // std::cout << "Listener is at: " << testingPos.x << std::endl;
}

void PersonController::updateVectors()
{
    camera_dir = normalize(position - camera_front); // sets the camera direction with a positive Z axis
    camera_right = normalize(cross(world_up, camera_dir));
}

void PersonController::updateInput(float deltaTime)
{
    float velocity = movespeed * deltaTime;
    
    bool rigidBodyCheck = false;
    
    // btRigidBody* hasRigidBody = this->getGameObject()->getComponent<RigidBody>()->getRigidBody();
    // btRigidBody* hasRigidBody = gameObject->getComponent<RigidBody>()->getRigidBody();
    btRigidBody* hasRigidBody = gameObject->getComponent<RigidBody>()->getRigidBody();
    // btTransform transform = hasRigidBody->getWorldTransform();

    btTransform transform;
    hasRigidBody->getMotionState()->getWorldTransform(transform);

    btVector3& origin = transform.getOrigin();
    position = {origin.x(), origin.y(), origin.z()};
    
    btVector3 force = {0,0,0};

    // auto thing = this->getGameObject()->getComponent<Transform>();
    // thing->rotation = 
    // update rotation
    float oldRotation = rotation;
    rotation += mouse_offset;
    // std::cout << "mouse offset: " << mouse_offset << std::endl;
    
    btVector3 angular_force_bt = {0,0,0};

    angular_force_bt = {0,mouse_offset,0};
    // if (mouse_offset > 0){

    //     angular_force_bt = {0,sensitivity,0};
    //     // std::cout << "mouse_offset positive" << std::endl;
    // } else if (mouse_offset < 0) {

    //     angular_force_bt = {0,-sensitivity,0};

    // }
    

    // keep rotation in 360 degree range
    if (rotation > 360.f)
        rotation -= 360.f;
    else if (rotation < -0.f)
        rotation += 360.f;
    mouse_offset = 0; // reset offset


    // update position based on current keypresses
    // using the camerafront vector allows to keep account of rotation automatically

    glm::vec3 oldPosition = position;

    if (key_fwd){

        position += velocity * camera_front;
        // force += velocity * camera_front_bt;
    }

    if (key_bwd){

        position -= velocity * camera_front;
        // force -= velocity * camera_front_bt;
    }
    if (key_left) {
        position -= velocity * cross(camera_front, world_up);
        // force -= velocity * cross(camera_front_bt, world_up);

    }
    if (key_right){
        position += velocity * cross(camera_front, world_up);
        // force += velocity * cross(camera_front, world_up);

    }
    if (key_flyUp){
        position += velocity * world_up; //works! fly's up

    }
    if (key_flyDown){
        position -= velocity * world_up; //flys down!

    }

    // Calculate the force to apply to the character
    glm::vec3 positionDifference = oldPosition - position;
    force = {positionDifference.x, positionDifference.y, positionDifference.z};
    
    

    // thing->position = position;
    origin = {position.x, position.y, position.z};
    transform.setOrigin(origin);
    // transform.setRotation();
    // hasRigidBody->getMotionState(); // Uses Motion State
    // hasRigidBody->getMotionState()->setWorldTransform(transform); // Uses Motion State
    // hasRigidBody->setWorldTransform(transform); // skips motion state - kinda works
    // hasRigidBody->setLinearVelocity(origin);
    // origin = origin /10;
    // std::cout << "force is :" << force.x() << ", " << force.y() << " " << force.z() << std::endl;
    btScalar totalForce = hasRigidBody->getLinearVelocity().length();
    // std::cout << "total Force is: " << totalForce << std::endl;
    if(totalForce <= 7) hasRigidBody->applyCentralImpulse(-force); //kinda works
    
    btVector3 currentVelocity = hasRigidBody->getLinearVelocity();

    // If force (i.e. walking) is not being applied, artificially change friction to slow down quickly
    if ((force.x() == 0 && force.z() == 0) 
    && (currentVelocity.x() != 0 && currentVelocity.y() != 0)
    && hasRigidBody->getFriction() != 5)
    {
        // hasRigidBody->applyDamping(0.2, 0.0);
        hasRigidBody->setFriction(5);
        // hasRigidBody->setDamping
        // std::cout << "applying dampening" << std::endl;
    } else if (hasRigidBody->getFriction() != 2) {
        // otherwise set a baselinie friction
        hasRigidBody->setFriction(2);
    }
    
    // xform.setRotation (btQuaternion (btVector3(0.0, 1.0, 0.0), m_turnAngle));
    // transform.setRotation(btQuaternion (btVector3(0.0, 1.0, 0.0), rotation));
    // hasRigidBody->setWorldTransform(transform);
    // std::cout << "angularforce is :" << angular_velocity_bt.x() << ", " << angular_velocity_bt.y() << " " << angular_velocity_bt.z() << std::endl;
    
    
    // hasRigidBody->applyTorque(angular_force_bt);
    hasRigidBody->setAngularVelocity(-angular_force_bt); // Kinda works
    
    // if jump key is pressed, jump!
    if (key_jump)
    {
        // Todo put in a vertical velocity check here, so there is no mid-air jumping
        hasRigidBody->applyCentralImpulse({0,jumpHeight,0});
    }
    
    
    // btQuaternion currentOrintation = hasRigidBody->getOrientation();
    // hasRigidBody->getMotionState()->getOrientation().setRotation(btVector3{0,1,0}, radians(rotation));
    // hasRigidBody->
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
    case SDLK_SPACE:
        key_jump = event.type == SDL_KEYDOWN;
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
        /*if(currentScene->gameManager->buildModeActive)
            camera->simpleRayCast(camera_front, this->tower, currentScene->getGameObjects());*/
    }
}

void PersonController::setInitialPosition(glm::vec2 position, float rotation)
{
    this->position = glm::vec3(position.x, 0, position.y);
    this->rotation = rotation;
}
