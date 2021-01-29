//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <SDL_events.h>
#include "Camera.hpp"

#include "Component.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"

#include "CollisionHandler.hpp"

#include <chrono>

class PersonController : public Component, public CollisionHandler {
public:
    explicit PersonController(GameObject* GameObject);
    virtual ~PersonController();

    void debugGUI() override ;

    void update(float deltaTime);
    void updateInput(float deltaTime);
    void updateVectors();
    void onKey(SDL_Event &event);
    void onMouse(SDL_Event &event);

    void setInitialPosition(glm::vec2 position, float rotation);

    float rotation = 0.f;
    glm::vec3 position;
    bool quit = false;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<GameObject> hand;
    std::shared_ptr<GameObject> tower;
    std::shared_ptr<GameObject> targetBlock;
    std::shared_ptr<Scene> currentScene;
    bool allowedToBuild = false;

    //------ customCollision Component Override

    void onCollision(size_t collisionId, RigidBody* other, glm::vec3 position, bool begin) override;
    // void onCollision(size_t collisionId, RigidBody *other, glm::vec3 position, bool begin);
    // void onCollisionEnd(size_t collisionId); 
    void onCollisionEnd(size_t collisionId) override; 

private:

    glm::vec3 world_up = glm::vec3(0, 1, 0);
    glm::vec3 camera_front;
    glm::vec3 camera_dir;
    glm::vec3 camera_right;

    glm::vec3 original_camera_dir;

    // float movespeed = 8.f;
    float movespeed = 20.f;
    // float sensitivity = 1.7f;
    float sensitivity = 100.7f;

    float jumpHeight = 5.f;

    // Footstep sound timing
    std::chrono::steady_clock::time_point start_footstep_lockout;
    int footstep_lockout_millisec = 300;

    // input stuff
    bool key_fwd = false;
    bool key_bwd = false;
    bool key_left = false;
    bool key_right = false;

    //turn these off for the real game
    bool key_flyUp = false;
    bool key_flyDown = false;

    bool key_jump = false;

    // float mouse_offset = 0.f;
    glm::vec2 mouse_offset = {0.0f, 0.0f};
    float pitch = 0;
};
