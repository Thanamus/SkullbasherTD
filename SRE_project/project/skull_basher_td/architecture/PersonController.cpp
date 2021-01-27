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
#include "SourceManager.hpp"

#include "Transform.hpp"

// btKinematicCharacterController includes
// #include "btKinematicCharacterController.h"
// #include "btGhostObject.h" // Aparrently doesn't exist
#include "RigidBody.hpp"

// #include "CollisionHandler.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

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

    start_footstep_lockout = std::chrono::steady_clock::now();

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

PersonController::~PersonController(){

}

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

    // // change to use transform instead of RigidBody 
    // auto myTransform = gameObject->getComponent<Transform>();
    
    // auto myPosition = myTransform->position;
    // auto myRotation = myTransform->rotation;

    camera_front = glm::normalize(vec3(cos(radians(rotation)), 0, sin(radians(rotation)))); // update camera "target" to match rotation
    // camera_front = glm::normalize(vec3(myRotation.x, 0.f, myRotation.z)); // nope!
    // camera_front = glm::normalize(vec3(myRotation.x, myRotation.y, myRotation.z)); // nope!
    // camera_front = {myRotation.x, 0.f, myRotation.z}; // Nope!
    // camera_front = glm::eulerAngles(myRotation); // Nope!
    
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
    position = {origin.x(), origin.y(), origin.z()}; // links origin and position
    
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
    
    // btQuaternion thing = transform.getRotation();
    // auto thingyy = thing.getY();
    // // btVector3 * thing_euler;
    // btScalar z;
    // btScalar y;
    // btScalar x;
    // thing.getEulerZYX(z,y,x); 

    // // btScalar test = radians(-rotation-180)/2;
    // btScalar test = radians(-rotation);
    // // xform.setRotation (btQuaternion (btVector3(0.0, 1.0, 0.0), m_turnAngle));
    // transform.setRotation(btQuaternion(btVector3(0.0, 1.0, 0.0), test));
    

    // std::cout << "thingyyy is: " << thingyy << std::endl;
    // std::cout << "y: " << y << std::endl;

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
    // if (key_flyUp){
    //     position += velocity * world_up; //works! fly's up
    // }
    // if (key_flyDown){
    //     position -= velocity * world_up; //flys down!
    // }

    // Calculate the force to apply to the character
    glm::vec3 positionDifference = oldPosition - position;
    force = {positionDifference.x, positionDifference.y, positionDifference.z};
    
    

    // thing->position = position;
    origin = {position.x, position.y, position.z}; // actually sets the origin

    // transform.setOrigin(origin);
    // transform.setRotation();
    // hasRigidBody->getMotionState(); // Uses Motion State
    // hasRigidBody->getMotionState()->setWorldTransform(transform); // Uses Motion State
    // hasRigidBody->setWorldTransform(transform); // skips motion state - kinda works
    // hasRigidBody->setLinearVelocity(origin);
    // origin = origin /10;
   
   
   
    // std::cout << "force is :" << force.x() << ", " << force.y() << " " << force.z() << std::endl;
    btScalar totalForce = hasRigidBody->getLinearVelocity().length();
    // std::cout << "total Force is: " << totalForce << std::endl;

    if(totalForce <= 7) {
        // if speed (total force) is less than 'some value', apply force (speed up)
        hasRigidBody->applyCentralImpulse(-force); //kinda works, have to set both origin and force
    } 
    
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
    
    // xform.setRotation (btQuaternion (btVector3(0.0, 1.0, 0.0), m_turnAngle)); // Original from Dynamic Contoller Demo
    // transform.setRotation(btQuaternion (btVector3(0.0, 1.0, 0.0), rotation));
    // hasRigidBody->setWorldTransform(transform);
    // std::cout << "angularforce is :" << angular_velocity_bt.x() << ", " << angular_velocity_bt.y() << " " << angular_velocity_bt.z() << std::endl;
    
    
    // hasRigidBody->applyTorque(angular_force_bt);
    // hasRigidBody->setAngularVelocity(-angular_force_bt); // Kinda works
    hasRigidBody->setAngularVelocity(-angular_force_bt*1.049); // It's a hack, but it works - might still be slightly off
    // hasRigidBody->setSpinningFriction(0);

    // std::cout << "vertical velocity is: " << currentVelocity.y() << std::endl;
    // if jump key is pressed, jump!
    if (key_jump)
    {
        if (currentVelocity.y() < 0.5) //helps a little
        {
            hasRigidBody->applyCentralImpulse({0,jumpHeight,0});
        }
        
        // Todo put in a vertical velocity check here, so there is no mid-air jumping
    }
    
    
    // btQuaternion currentOrintation = hasRigidBody->getOrientation();
    // hasRigidBody->getMotionState()->getOrientation().setRotation(btVector3{0,1,0}, radians(rotation));
    // hasRigidBody->getMotionState()->setWorldTransform(transform);
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


void PersonController::onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin){
    if (begin){
        std::string otherObjectName = other->getGameObject()->getName();
        std::cout << "Collision "<< collisionId <<" on "<< otherObjectName << " at "<<glm::to_string(position)<<std::endl;

        std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
        int time_elapsed_milli = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_footstep_lockout).count();

        std::cout << "time_elapsed :" << time_elapsed_milli << std::endl;        

        SourceManager * mySource = SourceManager::Get();
        //----- if colliding with ground, play ground sounds
        // works, but the sounds quickly pile up, maybe make a timer?
        if (time_elapsed_milli > footstep_lockout_millisec)
        {
            if (otherObjectName == "GrassBlock01D.obj")
            {
                mySource->playMyJam_global("gassy-footstep1.wav");
                // std::cout << "playing grassy footstep" << std::endl;
            } else if (otherObjectName == "Floor01.obj"){
                mySource->playMyJam_global("stepwood_2.wav");
            } else if (otherObjectName == "PathBlock01D.obj"){
                
            } else if (otherObjectName == "Bridge01D.obj"){
                
            }
            start_footstep_lockout = std::chrono::steady_clock::now();
        }
        
        
    }
}


void PersonController::onCollisionEnd(size_t collisionId) {
    std::cout << "Collision end "<<collisionId<<std::endl;
}